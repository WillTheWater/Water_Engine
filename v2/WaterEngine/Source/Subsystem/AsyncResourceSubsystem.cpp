// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/AsyncResourceSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    AsyncResourceSubsystem& AsyncResourceSubsystem::Get()
    {
        static AsyncResourceSubsystem Instance;
        return Instance;
    }

    AsyncResourceSubsystem::AsyncResourceSubsystem()
        : bRunning(true)
    {
        LoaderThread = std::thread(&AsyncResourceSubsystem::LoaderThreadLoop, this);
    }

    AsyncResourceSubsystem::~AsyncResourceSubsystem()
    {
        Shutdown();
    }

    void AsyncResourceSubsystem::Shutdown()
    {
        if (!bRunning) return;

        bRunning = false;
        PendingCV.notify_all();

        if (LoaderThread.joinable())
        {
            LoaderThread.join();
        }

        PendingRequests.clear();
        CompletedRequests.clear();
        AssetDirectory = nullptr;
    }

    void AsyncResourceSubsystem::SetAssetDirectory(shared<IAssetDirector> Directory)
    {
        AssetDirectory = Directory;
    }

    void AsyncResourceSubsystem::LoaderThreadLoop()
    {
        while (bRunning)
        {
            unique<IAsyncAssetRequest> Request;

            // Wait for work
            {
                std::unique_lock<std::mutex> Lock(PendingMutex);
                PendingCV.wait(Lock, [this] {
                    return !PendingRequests.empty() || !bRunning;
                    });

                if (!bRunning) break;

                // Sort by priority (High first)
                std::sort(PendingRequests.begin(), PendingRequests.end(),
                    [](const unique<IAsyncAssetRequest>& A, const unique<IAsyncAssetRequest>& B) {
                        return static_cast<int>(A->GetPriority()) > static_cast<int>(B->GetPriority());
                    });

                Request = std::move(PendingRequests.front());
                PendingRequests.erase(PendingRequests.begin());
            }

            // Load outside lock
            if (Request)
            {
                Request->Load(AssetDirectory);

                // Move to completed queue
                std::lock_guard<std::mutex> Lock(CompletedMutex);
                CompletedRequests.push_back(std::move(Request));
            }
        }
    }

    void AsyncResourceSubsystem::PollCompletedRequests()
    {
        list<unique<IAsyncAssetRequest>> ToFinalize;

        // Grab all completed
        {
            std::lock_guard<std::mutex> Lock(CompletedMutex);
            ToFinalize = std::move(CompletedRequests);
            CompletedRequests.clear();
        }

        // Finalize on main thread (GPU upload here)
        for (auto& Request : ToFinalize)
        {
            Request->Finalize();
        }
    }

    template<typename T>
    AssetHandle<T> AsyncResourceSubsystem::LoadAsyncInternal(const string& Path, AssetPriority Priority,
        std::function<bool(T&, const list<uint8>&)> Loader)
    {
        // Check cache first
        dictionary<string, weak<T>>* Cache = nullptr;
        if constexpr (std::is_same_v<T, texture>) Cache = &TextureCache;
        else if constexpr (std::is_same_v<T, font>) Cache = &FontCache;
        else if constexpr (std::is_same_v<T, soundBuffer>) Cache = &SoundCache;

        if (Cache)
        {
            auto It = Cache->find(Path);
            if (It != Cache->end())
            {
                if (auto Existing = It->second.lock())
                {
                    LOG("Async cache hit for: {}", Path);
                    return AssetHandle<T>(Existing);
                }
            }
        }

        AssetHandle<T> Handle;
        Handle.SetLoading();

        // Finalizer adds to cache when ready
        auto Finalizer = [this, Path, Cache](shared<T> Asset) {
            if (Cache) (*Cache)[Path] = Asset;
            };

        auto Request = std::make_unique<AssetRequest<T>>(Path, Priority, Handle, Loader, Finalizer);

        {
            std::lock_guard<std::mutex> Lock(PendingMutex);
            PendingRequests.push_back(std::move(Request));
        }
        PendingCV.notify_one();

        return Handle;
    }

    AssetHandle<texture> AsyncResourceSubsystem::LoadTextureAsync(const string& Path, AssetPriority Priority)
    {
        return LoadAsyncInternal<texture>(Path, Priority,
            [](texture& Tex, const list<uint8>& Data) {
                return Tex.loadFromMemory(Data.data(), Data.size());
            });
    }

    AssetHandle<font> AsyncResourceSubsystem::LoadFontAsync(const string& Path, AssetPriority Priority)
    {
        return LoadAsyncInternal<font>(Path, Priority,
            [](font& Fnt, const list<uint8>& Data) {
                return Fnt.openFromMemory(Data.data(), Data.size());
            });
    }

    AssetHandle<soundBuffer> AsyncResourceSubsystem::LoadSoundAsync(const string& Path, AssetPriority Priority)
    {
        return LoadAsyncInternal<soundBuffer>(Path, Priority,
            [](soundBuffer& Snd, const list<uint8>& Data) {
                return Snd.loadFromMemory(Data.data(), Data.size());
            });
    }

    // Sync methods - delegate to old ResourceSubsystem for now
    shared<texture> AsyncResourceSubsystem::LoadTextureSync(const string& Path)
    {
        return Asset().LoadTexture(Path);
    }

    shared<font> AsyncResourceSubsystem::LoadFontSync(const string& Path)
    {
        return Asset().LoadFont(Path);
    }

    shared<soundBuffer> AsyncResourceSubsystem::LoadSoundSync(const string& Path)
    {
        return Asset().LoadSound(Path);
    }

    void AsyncResourceSubsystem::GarbageCycle(float DeltaTime)
    {
        static float CleanupTimer = 0.0f;
        CleanupTimer += DeltaTime;

        if (CleanupTimer > 3.0f)
        {
            UnloadUnusedAssets();
            CleanupTimer = 0.0f;
        }
    }

    void AsyncResourceSubsystem::UnloadUnusedAssets()
    {
        size_t TexturesBefore = TextureCache.size();
        size_t FontsBefore = FontCache.size();
        size_t SoundsBefore = SoundCache.size();

        for (auto It = TextureCache.begin(); It != TextureCache.end();)
        {
            if (It->second.expired())
                It = TextureCache.erase(It);
            else
                ++It;
        }

        for (auto It = FontCache.begin(); It != FontCache.end();)
        {
            if (It->second.expired())
                It = FontCache.erase(It);
            else
                ++It;
        }

        for (auto It = SoundCache.begin(); It != SoundCache.end();)
        {
            if (It->second.expired())
                It = SoundCache.erase(It);
            else
                ++It;
        }

        size_t TotalFreed = (TexturesBefore - TextureCache.size()) +
            (FontsBefore - FontCache.size()) +
            (SoundsBefore - SoundCache.size());

        if (TotalFreed > 0)
        {
            LOG("AsyncAsset: Unloaded {} unused assets", TotalFreed);
        }
    }
}
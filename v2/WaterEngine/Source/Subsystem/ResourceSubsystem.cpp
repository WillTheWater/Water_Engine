// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"
#include "Utility/Assert.h"

namespace we
{
    ResourceSubsystem::ResourceSubsystem()
    {
        WorkerThread = std::thread(&ResourceSubsystem::LoaderThread, this);
    }

    ResourceSubsystem::~ResourceSubsystem()
    {
        Shutdown();
    }

    void ResourceSubsystem::Shutdown()
    {
        if (!bIsRunning) return;

        {
            std::lock_guard lock(PendingMutex);
            bIsRunning = false;
        }
        CV.notify_all();

        if (WorkerThread.joinable())
            WorkerThread.join();

        PendingRequests = {};
        CompletedRequests.clear();
        AssetDirector = nullptr;
    }

    void ResourceSubsystem::SetAssetDirectory(shared<IAssetDirector> Directory)
    {
        AssetDirector = std::move(Directory);
    }

    void ResourceSubsystem::LoaderThread()
    {
        while (true)
        {
            unique<LoadRequestBase> req;

            {
                std::unique_lock lock(PendingMutex);
                CV.wait(lock, [this] { return !PendingRequests.empty() || !bIsRunning; });

                if (!bIsRunning) return;

                req = std::move(PendingRequests.front());
                PendingRequests.pop();
            }

            if (req)
            {
                req->ExecuteLoad(AssetDirector);
                req->bIsDone = true;

                std::lock_guard lock(CompletedMutex);
                CompletedRequests.push_back(std::move(req));
            }
        }
    }

    void ResourceSubsystem::PollCompletedRequests()
    {
        vector<unique<LoadRequestBase>> ready;
        {
            std::lock_guard lock(CompletedMutex);
            ready = std::move(CompletedRequests);
        }

        for (auto& req : ready)
            req->ExecuteFinalize();
    }

    template<typename T>
    void ResourceSubsystem::LoadAsyncInternal(const string& Path,
        AssetSuccessCallback<T> OnSuccess,
        AssetErrorCallback OnError,
        std::function<bool(T&, const vector<uint8>&)> Loader)
    {
        auto req = std::make_unique<LoadRequest<T>>();
        req->Path = Path;
        req->OnSuccess = std::move(OnSuccess);
        req->OnError = std::move(OnError);
        req->Loader = std::move(Loader);

        {
            std::lock_guard lock(PendingMutex);
            PendingRequests.push(std::move(req));
        }
        CV.notify_one();
    }

    template<typename T>
    shared<T> ResourceSubsystem::LoadSyncInternal(const string& Path,
        std::function<bool(T&, const vector<uint8>&)> Loader)
    {
        if (!AssetDirector)
        {
            ERROR("No asset directory set");
            return nullptr;
        }

        vector<uint8> data;
        if (!AssetDirector->ReadFile(Path, data))
        {
            ERROR("Failed to read: {}", Path);
            return nullptr;
        }

        auto asset = make_shared<T>();
        if (!Loader(*asset, data))
        {
            ERROR("Failed to load: {}", Path);
            return nullptr;
        }

        return asset;
    }

    // ============================================================================
    // ASYNC API IMPLEMENTATIONS
    // ============================================================================

    void ResourceSubsystem::LoadTextureAsync(const string& Path,
        AssetSuccessCallback<texture> OnSuccess,
        AssetErrorCallback OnError)
    {
        LoadAsyncInternal<texture>(Path, std::move(OnSuccess), std::move(OnError),
            [](texture& t, const vector<uint8>& d) {
                return t.loadFromMemory(d.data(), d.size());
            });
    }

    void ResourceSubsystem::LoadFontAsync(const string& Path,
        AssetSuccessCallback<font> OnSuccess,
        AssetErrorCallback OnError)
    {
        LoadAsyncInternal<font>(Path, std::move(OnSuccess), std::move(OnError),
            [](font& f, const vector<uint8>& d) {
                return f.openFromMemory(d.data(), d.size());
            });
    }

    void ResourceSubsystem::LoadSoundAsync(const string& Path,
        AssetSuccessCallback<soundBuffer> OnSuccess,
        AssetErrorCallback OnError)
    {
        LoadAsyncInternal<soundBuffer>(Path, std::move(OnSuccess), std::move(OnError),
            [](soundBuffer& s, const vector<uint8>& d) {
                return s.loadFromMemory(d.data(), d.size());
            });
    }

    void ResourceSubsystem::LoadRawDataAsync(const string& Path,
        AssetSuccessCallback<vector<uint8>> OnSuccess,
        AssetErrorCallback OnError)
    {
        LoadAsyncInternal<vector<uint8>>(Path, std::move(OnSuccess), std::move(OnError),
            [](vector<uint8>& out, const vector<uint8>& in) {
                out = in;
                return true;
            });
    }

    // ============================================================================
    // SYNC API IMPLEMENTATIONS
    // ============================================================================

    shared<texture> ResourceSubsystem::LoadTextureSync(const string& Path)
    {
        return LoadSyncInternal<texture>(Path,
            [](texture& t, const vector<uint8>& d) {
                return t.loadFromMemory(d.data(), d.size());
            });
    }

    shared<font> ResourceSubsystem::LoadFontSync(const string& Path)
    {
        return LoadSyncInternal<font>(Path,
            [](font& f, const vector<uint8>& d) {
                return f.openFromMemory(d.data(), d.size());
            });
    }

    shared<soundBuffer> ResourceSubsystem::LoadSoundSync(const string& Path)
    {
        return LoadSyncInternal<soundBuffer>(Path,
            [](soundBuffer& s, const vector<uint8>& d) {
                return s.loadFromMemory(d.data(), d.size());
            });
    }

    shared<music> ResourceSubsystem::LoadMusicSync(const string& Path)
    {
        // Music needs persistent data
        auto data = LoadRawDataSync(Path);
        if (!data) return nullptr;

        // TODO: Implement MusicMemoryStream
        LOG("LoadMusicSync not fully implemented - needs MusicMemoryStream");
        return nullptr;
    }

    shared<shader> ResourceSubsystem::LoadShaderSync(const string& Path, shader::Type Type)
    {
        auto data = LoadRawDataSync(Path);
        if (!data) return nullptr;

        string source(data->begin(), data->end());
        auto s = make_shared<shader>();
        if (!s->loadFromMemory(source, Type))
            return nullptr;
        return s;
    }

    shared<vector<uint8>> ResourceSubsystem::LoadRawDataSync(const string& Path)
    {
        return LoadSyncInternal<vector<uint8>>(Path,
            [](vector<uint8>& out, const vector<uint8>& in) {
                out = in;
                return true;
            });
    }

    shared<texture> ResourceSubsystem::GetPlaceholderTexture()
    {
        if (!PlaceholderTexture)
        {
            sf::Image img;
            img.resize({ 1, 1 }, sf::Color::White);
            PlaceholderTexture = make_shared<texture>();
            (void)PlaceholderTexture->loadFromImage(img);
        }
        return PlaceholderTexture;
    }
}

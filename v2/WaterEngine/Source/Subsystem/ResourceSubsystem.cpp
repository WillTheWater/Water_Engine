// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"
#include "Utility/Assert.h"

namespace we
{
    ResourceSubsystem* ResourceSubsystem::RSubInstance = nullptr;

    ResourceSubsystem& ResourceSubsystem::Get()
    {
        VERIFY(RSubInstance && "ResourceSubsystem not initialized!");
        return *RSubInstance;
    }

    ResourceSubsystem::ResourceSubsystem()
    {
        VERIFY(!RSubInstance && "ResourceSubsystem already exists!");
        RSubInstance = this;
        AssetThread = std::thread(&ResourceSubsystem::LoaderThread, this);
    }

    ResourceSubsystem::~ResourceSubsystem()
    {
        Shutdown();
        RSubInstance = nullptr;
    }

    void ResourceSubsystem::Shutdown()
    {
        if (!bIsRunning) return;

        {
            std::lock_guard lock(PendingMutex);
            bIsRunning = false;
        }
        CV.notify_all();

        if (AssetThread.joinable())
            AssetThread.join();

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
            unique<RequestBase> req;

            {
                std::unique_lock lock(PendingMutex);
                CV.wait(lock, [this] { return !PendingRequests.empty() || !bIsRunning; });

                if (!bIsRunning) return;

                // Pop highest priority request (O(log n) heap operation)
                req = std::move(const_cast<unique<RequestBase>&>(PendingRequests.top()));
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
        vector<unique<RequestBase>> ready;
        {
            std::lock_guard lock(CompletedMutex);
            ready = std::move(CompletedRequests);
        }

        for (auto& req : ready)
            req->ExecuteFinalize();
    }

    template<typename T>
    AssetHandle<T> ResourceSubsystem::LoadAsyncInternal(const string& Path, AssetPriority Priority,
        std::function<bool(T&, const vector<uint8>&)> Loader)
    {
        AssetHandle<T> handle;

        auto req = std::make_unique<Request<T>>();
        req->Path = Path;
        req->Priority = Priority;
        req->Handle = handle;
        req->Loader = std::move(Loader);

        {
            std::lock_guard lock(PendingMutex);
            PendingRequests.push(std::move(req));
        }
        CV.notify_one();

        return handle;
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
    // Public API
    // ============================================================================

    AssetHandle<texture> ResourceSubsystem::LoadTextureAsync(const string& Path, AssetPriority Priority)
    {
        if (auto cached = FindInCache<texture>(Path, TextureCache))
            return AssetHandle<texture>(cached);

        return LoadAsyncInternal<texture>(Path, Priority,
            [](texture& t, const vector<uint8>& d) {
                return t.loadFromMemory(d.data(), d.size());
            });
    }

    AssetHandle<font> ResourceSubsystem::LoadFontAsync(const string& Path, AssetPriority Priority)
    {
        if (auto cached = FindInCache<font>(Path, FontCache))
            return AssetHandle<font>(cached);

        return LoadAsyncInternal<font>(Path, Priority,
            [this, Path](font& f, const vector<uint8>& d) {
                // Store data persistently for fonts
                auto dataPtr = make_shared<vector<uint8>>(d);
                FontDataCache[Path] = dataPtr;
                return f.openFromMemory(dataPtr->data(), dataPtr->size());
            });
    }

    AssetHandle<soundBuffer> ResourceSubsystem::LoadSoundAsync(const string& Path, AssetPriority Priority)
    {
        if (auto cached = FindInCache<soundBuffer>(Path, SoundCache))
            return AssetHandle<soundBuffer>(cached);

        return LoadAsyncInternal<soundBuffer>(Path, Priority,
            [](soundBuffer& s, const vector<uint8>& d) {
                return s.loadFromMemory(d.data(), d.size());
            });
    }

    shared<texture> ResourceSubsystem::LoadTextureSync(const string& Path)
    {
        if (auto cached = FindInCache<texture>(Path, TextureCache))
            return cached;

        auto asset = LoadSyncInternal<texture>(Path,
            [](texture& t, const vector<uint8>& d) {
                return t.loadFromMemory(d.data(), d.size());
            });

        if (asset) AddToCache(Path, asset, TextureCache);
        return asset;
    }

    shared<font> ResourceSubsystem::LoadFontSync(const string& Path)
    {
        if (auto cached = FindInCache<font>(Path, FontCache))
            return cached;

        // Load data first
        if (!AssetDirector) return nullptr;

        auto& data = FontDataCache[Path];
        if (!data)
        {
            vector<uint8> raw;
            if (!AssetDirector->ReadFile(Path, raw)) return nullptr;
            data = make_shared<vector<uint8>>(std::move(raw));
        }

        auto font = make_shared<we::font>();
        if (!font->openFromMemory(data->data(), data->size()))
        {
            ERROR("Failed to load font: {}", Path);
            return nullptr;
        }

        AddToCache(Path, font, FontCache);
        return font;
    }

    shared<soundBuffer> ResourceSubsystem::LoadSoundSync(const string& Path)
    {
        if (auto cached = FindInCache<soundBuffer>(Path, SoundCache))
            return cached;

        auto asset = LoadSyncInternal<soundBuffer>(Path,
            [](soundBuffer& s, const vector<uint8>& d) {
                return s.loadFromMemory(d.data(), d.size());
            });

        if (asset) AddToCache(Path, asset, SoundCache);
        return asset;
    }

    shared<music> ResourceSubsystem::LoadMusic(const string& Path)
    {
        if (!AssetDirector)
        {
            ERROR("No asset directory for: {}", Path);
            return nullptr;
        }

        auto& data = MusicDataCache[Path];
        if (!data)
        {
            vector<uint8> raw;
            if (!AssetDirector->ReadFile(Path, raw))
            {
                ERROR("Failed to read music: {}", Path);
                return nullptr;
            }
            data = make_shared<vector<uint8>>(std::move(raw));
        }

        auto stream = make_shared<MusicMemoryStream>(data);
        auto music = make_shared<we::music>();

        if (!music->openFromStream(*stream))
        {
            ERROR("Failed to open music stream: {}", Path);
            return nullptr;
        }

        MusicStreamCache[Path] = stream;
        return music;
    }

    shared<shader> ResourceSubsystem::LoadShader(const string& Path, shader::Type Type)
    {
        if (!AssetDirector) return nullptr;

        vector<uint8> data;
        if (!AssetDirector->ReadFile(Path, data)) return nullptr;

        string source(data.begin(), data.end());
        auto shader = make_shared<we::shader>();

        if (!shader->loadFromMemory(source, Type))
            return nullptr;

        return shader;
    }

    shared<texture> ResourceSubsystem::GetPlaceholderTexture()
    {
        if (!PlaceholderTexture)
        {
            // Create a 1x1 white texture using Image
            sf::Image img;
            img.resize({ 1, 1 }, sf::Color::White);
            PlaceholderTexture = make_shared<texture>();
            PlaceholderTexture->loadFromImage(img);
        }
        return PlaceholderTexture;
    }

    void ResourceSubsystem::GarbageCycle(float DeltaTime)
    {
        GarbageCycleTimer += DeltaTime;
        if (GarbageCycleTimer < 3.0f) return;
        GarbageCycleTimer = 0.0f;

        auto cleanup = [](auto& cache) {
            for (auto it = cache.begin(); it != cache.end();)
                it = it->second.expired() ? cache.erase(it) : std::next(it);
            };

        cleanup(TextureCache);
        cleanup(FontCache);
        cleanup(SoundCache);
    }
}
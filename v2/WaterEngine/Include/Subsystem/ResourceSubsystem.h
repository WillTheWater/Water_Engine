// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/AudioTypes.h"
#include "Core/EngineConfig.h"
#include "Interface/IAssetDirector.h"
#include "Core/MusicMemoryStream.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace we
{
    // =============================================================================
    // ASYNC LOAD CALLBACK TYPES
    // =============================================================================

    template<typename T>
    using AssetSuccessCallback = std::function<void(shared<T>)>;

    using AssetErrorCallback = std::function<void(const string&)>;

    // =============================================================================
    // BASE REQUEST (Type-erased for queue)
    // =============================================================================

    struct LoadRequestBase
    {
        virtual ~LoadRequestBase() = default;
        virtual void ExecuteLoad(shared<IAssetDirector> Assets) = 0;
        virtual void ExecuteFinalize() = 0;
        virtual const string& GetPath() const = 0;

        string Path;
        bool bIsDone = false;
        AssetErrorCallback OnError;
    };

    // =============================================================================
    // TYPED REQUEST
    // =============================================================================

    template<typename T>
    struct LoadRequest : LoadRequestBase
    {
        AssetSuccessCallback<T> OnSuccess;
        shared<T> Result;
        vector<uint8> Data;
        std::function<bool(T&, const vector<uint8>&)> Loader;

        const string& GetPath() const override { return Path; }

        void ExecuteLoad(shared<IAssetDirector> Assets) override
        {
            if (Assets)
                Assets->ReadFile(Path, Data);
        }

        void ExecuteFinalize() override
        {
            if (Data.empty())
            {
                if (OnError) OnError("Failed to read file: " + Path);
                return;
            }

            auto asset = make_shared<T>();
            if (Loader(*asset, Data))
            {
                if (OnSuccess) OnSuccess(asset);
            }
            else
            {
                if (OnError) OnError("Failed to parse asset: " + Path);
            }
        }
    };

    // =============================================================================
    // RESOURCE SUBSYSTEM
    // =============================================================================

    class ResourceSubsystem
    {
    public:
        ResourceSubsystem();
        ~ResourceSubsystem();

        void SetAssetDirectory(shared<IAssetDirector> Directory);
        shared<IAssetDirector> GetAssetDirectory() const { return AssetDirector; }
        void Shutdown();

        // Called once per frame from main thread
        void PollCompletedRequests();

        // =========================================================================
        // ASYNC LOADING (Callbacks fire on main thread)
        // =========================================================================

        void LoadTextureAsync(const string& Path, 
            AssetSuccessCallback<texture> OnSuccess,
            AssetErrorCallback OnError = nullptr);

        void LoadFontAsync(const string& Path,
            AssetSuccessCallback<font> OnSuccess,
            AssetErrorCallback OnError = nullptr);

        void LoadSoundAsync(const string& Path,
            AssetSuccessCallback<soundBuffer> OnSuccess,
            AssetErrorCallback OnError = nullptr);

        void LoadRawDataAsync(const string& Path,
            AssetSuccessCallback<vector<uint8>> OnSuccess,
            AssetErrorCallback OnError = nullptr);

        // =========================================================================
        // SYNC LOADING (Direct, blocks calling thread)
        // =========================================================================

        shared<texture> LoadTextureSync(const string& Path);
        shared<font> LoadFontSync(const string& Path);
        shared<soundBuffer> LoadSoundSync(const string& Path);
        shared<shader> LoadShaderSync(const string& Path, shader::Type Type);
        shared<vector<uint8>> LoadRawDataSync(const string& Path);
        
        // Music loading (returns music with persistent stream for pak support)
        shared<music> LoadMusic(const string& Path);

        // =========================================================================
        // PLACEHOLDER
        // =========================================================================

        shared<texture> GetPlaceholderTexture();

    private:
        void LoaderThread();

        template<typename T>
        void LoadAsyncInternal(const string& Path,
            AssetSuccessCallback<T> OnSuccess,
            AssetErrorCallback OnError,
            std::function<bool(T&, const vector<uint8>&)> Loader);

        template<typename T>
        shared<T> LoadSyncInternal(const string& Path,
            std::function<bool(T&, const vector<uint8>&)> Loader);

    private:
        shared<IAssetDirector> AssetDirector;

        std::thread WorkerThread;
        std::mutex PendingMutex;
        std::mutex CompletedMutex;
        std::condition_variable CV;
        bool bIsRunning = true;

        std::queue<unique<LoadRequestBase>> PendingRequests;
        vector<unique<LoadRequestBase>> CompletedRequests;

        // Music cache (needs persistent data + stream)
        dictionary<string, shared<vector<uint8>>> MusicDataCache;
        dictionary<string, shared<MusicMemoryStream>> MusicStreamCache;
        dictionary<string, weak<music>> MusicCache;

        shared<texture> PlaceholderTexture;
    };
}

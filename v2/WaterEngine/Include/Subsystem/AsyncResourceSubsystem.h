// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Interface/IAssetDirector.h"

#include <thread>
#include <mutex>
#include <condition_variable>

namespace we
{
    enum class AssetState : uint8
    {
        Loading,
        Ready,
        Failed
    };

    enum class AssetPriority : uint8
    {
        Low,      // Background scenery
        Normal,   // Standard gameplay assets
        High      // UI, player character, critical
    };

    template<typename T>
    class AssetHandle
    {
    public:
        AssetHandle() : Internal(make_shared<InternalState>()) {}
        explicit AssetHandle(shared<T> Asset) : Internal(make_shared<InternalState>())
        {
            Internal->Asset = Asset;
            Internal->Status = AssetState::Ready;
        }

        bool IsValid() const { return Internal->Status != AssetState::Failed; }
        bool IsReady() const { return Internal->Status == AssetState::Ready; }
        bool IsLoading() const { return Internal->Status == AssetState::Loading; }
        AssetState GetState() const { return Internal->Status; }

        T* Get() const { return Internal->Asset.get(); }
        T& operator*() const { return *Internal->Asset; }
        T* operator->() const { return Internal->Asset.get(); }

        explicit operator bool() const { return IsReady(); }
        void SetLoading() { Internal->Status = AssetState::Loading; }
        void SetReady(shared<T> Asset) { Internal->Asset = Asset; Internal->Status = AssetState::Ready; }
        void SetFailed() { Internal->Status = AssetState::Failed; }

    private:
        friend class AsyncResourceSubsystem;


        struct InternalState {
            shared<T> Asset;
            AssetState Status = AssetState::Loading;
        };

        shared<InternalState> Internal;
    };

    // Base class for type-erased asset requests
    class IAsyncAssetRequest
    {
    public:
        virtual ~IAsyncAssetRequest() = default;
        virtual void Load(shared<IAssetDirector> AssetDir) = 0;
        virtual void Finalize() = 0;
        virtual AssetPriority GetPriority() const = 0;
        virtual bool IsComplete() const = 0;
    };

    // Concrete request for specific asset type
    template<typename T>
    class AssetRequest : public IAsyncAssetRequest
    {
    public:
        using LoadFunc = std::function<bool(T&, const list<uint8>&)>;
        using FinalizeFunc = std::function<void(shared<T>)>;

        AssetRequest(string Path, AssetPriority Priority, AssetHandle<T> Handle, LoadFunc Loader, FinalizeFunc Finalizer)
            : Path(std::move(Path)), Priority(Priority), Handle(std::move(Handle)),
            Loader(std::move(Loader)), Finalizer(std::move(Finalizer)),
            bComplete(false), bSuccess(false)
        {
        }

        void Load(shared<IAssetDirector> AssetDir) override
        {
            list<uint8> FileData;
            if (!AssetDir || !AssetDir->ReadFile(Path, FileData))
            {
                bComplete = true;
                bSuccess = false;
                return;
            }

            LoadedData = std::move(FileData);
            bComplete = true;
            bSuccess = true;
        }

        void Finalize() override
        {
            if (!bSuccess)
            {
                Handle.SetFailed();
                return;
            }

            auto Asset = make_shared<T>();
            if (Loader(*Asset, LoadedData))
            {
                Handle.SetReady(Asset);
                Finalizer(Asset);
            }
            else
            {
                Handle.SetFailed();
            }
        }

        AssetPriority GetPriority() const override { return Priority; }
        bool IsComplete() const override { return bComplete; }

    private:
        string Path;
        AssetPriority Priority;
        AssetHandle<T> Handle;
        LoadFunc Loader;
        FinalizeFunc Finalizer;

        list<uint8> LoadedData;
        bool bComplete;
        bool bSuccess;
    };

    class AsyncResourceSubsystem
    {
    public:
        static AsyncResourceSubsystem& Get();

        void SetAssetDirectory(shared<IAssetDirector> Directory);
        void Shutdown();

        // Async loading methods
        AssetHandle<texture> LoadTextureAsync(const string& Path, AssetPriority Priority = AssetPriority::Normal);
        AssetHandle<font> LoadFontAsync(const string& Path, AssetPriority Priority = AssetPriority::Normal);
        AssetHandle<soundBuffer> LoadSoundAsync(const string& Path, AssetPriority Priority = AssetPriority::Normal);

        // Synchronous loading (blocking, use with caution)
        shared<texture> LoadTextureSync(const string& Path);
        shared<font> LoadFontSync(const string& Path);
        shared<soundBuffer> LoadSoundSync(const string& Path);

        // Call every frame from main thread
        void PollCompletedRequests();

        // Cache management
        void GarbageCycle(float DeltaTime);
        void UnloadUnusedAssets();

    private:
        AsyncResourceSubsystem();
        ~AsyncResourceSubsystem();
        void LoaderThreadLoop();

        template<typename T>
        AssetHandle<T> LoadAsyncInternal(const string& Path, AssetPriority Priority,
            std::function<bool(T&, const list<uint8>&)> Loader);

    private:
        shared<IAssetDirector> AssetDirectory;

        // Threading
        std::thread LoaderThread;
        bool bRunning;

        // Job queue (loader thread consumes)
        list<unique<IAsyncAssetRequest>> PendingRequests;
        std::mutex PendingMutex;
        std::condition_variable PendingCV;

        // Completion queue (main thread finalizes)
        list<unique<IAsyncAssetRequest>> CompletedRequests;
        std::mutex CompletedMutex;

        // Caches - shared_ptr keeps assets alive while handles exist
        dictionary<string, weak<texture>> TextureCache;
        dictionary<string, weak<font>> FontCache;
        dictionary<string, weak<soundBuffer>> SoundCache;
    };

    inline AsyncResourceSubsystem& AsyncAsset() { return AsyncResourceSubsystem::Get(); }
}
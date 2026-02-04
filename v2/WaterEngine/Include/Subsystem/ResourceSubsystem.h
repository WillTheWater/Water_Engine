// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Interface/IAssetDirector.h"
#include "Utility/Log.h"
#include <queue>
#include <mutex>
#include <functional>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace we
{
    class ResourceSubsystem
    {
    public:
        static ResourceSubsystem& Get();
        ~ResourceSubsystem();

        void SetAssetDirectory(shared<IAssetDirector> Directory);

        shared<texture> LoadTexture(const string& Path);
        shared<font> LoadFont(const string& Path);
        shared<soundBuffer> LoadSound(const string& Path);

        void ProcessDeferredFlush(size_t MaxPerFrame);
        void Shutdown();

    protected:
        ResourceSubsystem();

    private:
        shared<IAssetDirector> AssetDirectory;

        dictionary<string, weak<texture>> LoadedTextures;
        dictionary<string, weak<font>> LoadedFonts;
        dictionary<string, weak<soundBuffer>> LoadedSounds;
        dictionary<string, list<uint8>> FontBuffers;

        std::queue<std::function<void()>> DeferredFlush;
        std::mutex FlushMutex;

        std::thread CleanupThread;
        std::atomic<bool> bRunning{ true };
        std::condition_variable DeletionCV;
        std::mutex CVMutex;

    private:
        ResourceSubsystem(const ResourceSubsystem&) = delete;
        ResourceSubsystem(ResourceSubsystem&&) = delete;
        ResourceSubsystem& operator=(const ResourceSubsystem&) = delete;
        ResourceSubsystem& operator=(ResourceSubsystem&&) = delete;

    private:
        static unique<ResourceSubsystem> AssetManager;
        void CleanupLoop();

        template<typename Asset>
        shared<Asset> CreateTrackedAsset(const string& Path, shared<Asset> RawAsset);


        template<typename Asset, typename Dictionary, typename LoaderFunc>
        shared<Asset> LoadAsset(const string& Path, Dictionary& Cache, LoaderFunc&& Loader);
    };

    inline ResourceSubsystem& Asset() { return ResourceSubsystem::Get(); }

    template<typename Asset>
    inline shared<Asset> ResourceSubsystem::CreateTrackedAsset(const string& Path, shared<Asset> RawAsset)
    {
        LOG("CreateTrackedAsset: {}", Path);

        // Holder for RawAsset
        struct Holder { shared<Asset> AssetPtr; };

        auto HolderPtr = std::make_shared<Holder>();
        HolderPtr->AssetPtr = std::move(RawAsset); // Transfers ownership to holder

        Asset* RawPtr = HolderPtr->AssetPtr.get();

        auto Deleter = [this, Path, HolderPtr](Asset* Ptr)
            {
                // HolderPtr keeps asset alive until deleter runs
                std::lock_guard<std::mutex> Lock(FlushMutex);
                DeferredFlush.push([HolderPtr, Path]() {
                    // HolderPtr destroyed here, actually deletes asset
                    LOG("Async delete executing for {}", Path);
                    });
                DeletionCV.notify_one();
            };

        return shared<Asset>(RawPtr, Deleter);
    }

    template<typename Asset, typename Dictionary, typename LoaderFunc>
    inline shared<Asset> ResourceSubsystem::LoadAsset(const string& Path, Dictionary& Cache, LoaderFunc&& Loader)
    {
        auto it = Cache.find(Path);
        if (it != Cache.end())
        {
            if (auto Existing = it->second.lock())
            {
                return Existing;
            }
        }

        if (!AssetDirectory)
        {
            ERROR("LoadAsset Failed: AssetDirectory is NULL for {}", Path);
            return nullptr;
        }

        list<uint8> FileData;
        if (!AssetDirectory->ReadFile(Path, FileData))
        {
            ERROR("LoadAsset Failed: Could not read file {}", Path);
            return nullptr;
        }

        auto NewAsset = make_shared<Asset>();
        if (!Loader(*NewAsset, FileData))
        {
            ERROR("LoadAsset Failed: Loader (SFML) failed for {}", Path);
            return nullptr;
        }

        auto TrackedAsset = CreateTrackedAsset(Path, std::move(NewAsset));
        Cache[Path] = TrackedAsset;
        return TrackedAsset;
    }
}
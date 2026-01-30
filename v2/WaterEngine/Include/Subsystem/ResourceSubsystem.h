// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Interface/IAssetDirector.h"
#include "Utility/Log.h"

namespace we
{
	class ResourceSubsystem
	{
    public:
        static ResourceSubsystem& Get();

        void SetAssetDirectory(shared<IAssetDirector> Directory);

        shared<texture> LoadTexture(const string& Path);
        shared<font> LoadFont(const string& Path);
        shared<soundBuffer> LoadSound(const string& Path);

        template<typename Dictionary>
        void GarbageCollect(Dictionary& Cache);

    protected:
        ResourceSubsystem();

    private:
        shared<IAssetDirector> AssetDirectory;

        dictionary<string, shared<texture>> LoadedTextures;
        dictionary<string, list<uint8>> FontBuffers;
        dictionary<string, shared<font>> LoadedFonts;
        dictionary<string, shared<soundBuffer>> LoadedSounds;

    private:
        ResourceSubsystem(const ResourceSubsystem&) = delete;
        ResourceSubsystem(ResourceSubsystem&&) = delete;
        ResourceSubsystem& operator=(const ResourceSubsystem&) = delete;
        ResourceSubsystem& operator=(ResourceSubsystem&&) = delete;

    private:
        static unique<ResourceSubsystem> AssetManager;

        template<typename Asset, typename Dictionary, typename LoaderFunc>
        shared<Asset> LoadAsset(const string& Path, Dictionary& Cache, LoaderFunc&& Loader);
	};

    inline ResourceSubsystem& Asset() { return ResourceSubsystem::Get(); }

    template<typename Dictionary>
    inline void ResourceSubsystem::GarbageCollect(Dictionary& Cache)
    {
        for (auto it = Cache.begin(); it != Cache.end();)
        {
            if (it->second.unique())
            {
                it = Cache.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    template<typename Asset, typename Dictionary, typename LoaderFunc>
    inline shared<Asset> ResourceSubsystem::LoadAsset(const string& Path, Dictionary& Cache, LoaderFunc&& Loader)
    {
        auto it = Cache.find(Path);
        if (it != Cache.end()) return it->second;

        if (!AssetDirectory)
        {
            ERROR("LoadAsset Failed: AssetDirectory is NULL for {}", Path); // Check 1
            return nullptr;
        }

        list<uint8> FileData;
        if (!AssetDirectory->ReadFile(Path, FileData))
        {
            ERROR("LoadAsset Failed: Could not read file {}", Path); // Check 2
            return nullptr;
        }

        auto AssetPtr = std::make_shared<Asset>();
        if (!Loader(*AssetPtr, FileData))
        {
            ERROR("LoadAsset Failed: Loader (SFML) failed for {}", Path); // Check 3
            return nullptr;
        }

        Cache.emplace(Path, AssetPtr);
        return AssetPtr;
    }
}
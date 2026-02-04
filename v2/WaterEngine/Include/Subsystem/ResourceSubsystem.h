// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"
#include "Interface/IAssetDirector.h"

namespace we
{
    class ResourceSubsystem
    {
    public:
        static ResourceSubsystem& Get();

        void SetAssetDirectory(shared<IAssetDirector> Directory);
        void Shutdown();

    public:
        shared<texture> LoadTexture(const string& Path);
        shared<font> LoadFont(const string& Path);
        shared<soundBuffer> LoadSound(const string& Path);

    private:
        ResourceSubsystem() = default;
        ~ResourceSubsystem() = default;

    private:
        shared<IAssetDirector> AssetDirectory;

        dictionary<string, weak<texture>> Textures;
        dictionary<string, weak<font>> Fonts;
        dictionary<string, weak<soundBuffer>> Sounds;
        dictionary<string, list<uint8>> FontData;

    private:
        template<typename Asset, typename Cache>
        shared<Asset> Load(const string& Path, Cache& AssetCache, auto&& LoadFunc);
    };

    inline ResourceSubsystem& Asset() { return ResourceSubsystem::Get(); }
}
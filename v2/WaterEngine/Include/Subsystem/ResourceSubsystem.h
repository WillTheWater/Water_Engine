// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/AudioTypes.h"
#include "Interface/IAssetDirector.h"
#include <SFML/System/InputStream.hpp>

namespace we
{
    class MusicMemoryStream;

    class ResourceSubsystem
    {
    public:
        static ResourceSubsystem& Get();

        void SetAssetDirectory(shared<IAssetDirector> Directory);
        void Shutdown();

        shared<texture> LoadTexture(const string& Path);
        shared<font> LoadFont(const string& Path);
        shared<soundBuffer> LoadSound(const string& Path);
        shared<music> LoadMusic(const string& Path);

        shared<shader> LoadShader(const string& Path, shader::Type Type);

    private:
        ResourceSubsystem() = default;
        ~ResourceSubsystem() = default;

        shared<IAssetDirector> AssetDirectory;

        dictionary<string, shared<texture>> Textures;
        dictionary<string, shared<font>> Fonts;
        dictionary<string, shared<soundBuffer>> Sounds;
        dictionary<string, vector<uint8>> FontData;
        dictionary<string, shared<vector<uint8>>> MusicData;
        dictionary<string, shared<MusicMemoryStream>> MusicStreams;

        template<typename Asset, typename Cache>
        shared<Asset> Load(const string& Path, Cache& AssetCache, auto&& LoadFunc);
    };

    inline ResourceSubsystem& Asset() { return ResourceSubsystem::Get(); }

    class MusicMemoryStream : public sf::InputStream
    {
    public:
        explicit MusicMemoryStream(shared<vector<uint8>> InData);

        optional<ulong> read(void* data, ulong size) override;
        optional<ulong> seek(ulong position) override;
        optional<ulong> tell() override;
        optional<ulong> getSize() override;

    private:
        shared<vector<uint8>> Data;
        ulong Pos;
    };
}
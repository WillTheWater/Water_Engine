// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h
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
        shared<sf::Music> LoadMusic(const string& Path);

        shared<shader> LoadShader(const string& Path, shader::Type Type);

    private:
        ResourceSubsystem() = default;
        ~ResourceSubsystem() = default;

        shared<IAssetDirector> AssetDirectory;

        dictionary<string, shared<texture>> Textures;
        dictionary<string, shared<font>> Fonts;
        dictionary<string, shared<soundBuffer>> Sounds;
        dictionary<string, list<uint8>> FontData;
        dictionary<string, shared<list<uint8>>> MusicData;
        dictionary<string, shared<MusicMemoryStream>> MusicStreams;

        template<typename Asset, typename Cache>
        shared<Asset> Load(const string& Path, Cache& AssetCache, auto&& LoadFunc);
    };

    inline ResourceSubsystem& Asset() { return ResourceSubsystem::Get(); }

    class MusicMemoryStream : public sf::InputStream
    {
    public:
        explicit MusicMemoryStream(shared<list<uint8>> InData);

        optional<usize> read(void* data, usize size) override;
        optional<usize> seek(usize position) override;
        optional<usize> tell() override;
        optional<usize> getSize() override;

    private:
        shared<list<uint8>> Data;
        usize Pos;
    };
}
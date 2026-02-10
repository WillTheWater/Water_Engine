// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    MusicMemoryStream::MusicMemoryStream(shared<list<uint8>> InData)
        : Data(std::move(InData)), Pos(0)
    {
    }

    std::optional<usize> MusicMemoryStream::read(void* data, usize size)
    {
        usize Available = Data->size() - Pos;
        usize ToRead = (size < Available) ? size : Available;
        if (ToRead > 0)
        {
            std::memcpy(data, Data->data() + Pos, ToRead);
            Pos += ToRead;
        }
        return ToRead;
    }

    std::optional<usize> MusicMemoryStream::seek(usize position)
    {
        Pos = (position > Data->size()) ? Data->size() : position;
        return Pos;
    }

    std::optional<usize> MusicMemoryStream::tell()
    {
        return Pos;
    }

    std::optional<usize> MusicMemoryStream::getSize()
    {
        return Data->size();
    }

    ResourceSubsystem& ResourceSubsystem::Get()
    {
        static ResourceSubsystem Instance;
        return Instance;
    }

    void ResourceSubsystem::Shutdown()
    {
        Textures.clear();
        Fonts.clear();
        Sounds.clear();
        FontData.clear();
        MusicData.clear();
        MusicStreams.clear();
        AssetDirectory = nullptr;
    }

    void ResourceSubsystem::SetAssetDirectory(shared<IAssetDirector> Directory)
    {
        AssetDirectory = Directory;
    }

    template<typename Asset, typename Cache>
    shared<Asset> ResourceSubsystem::Load(const string& Path, Cache& AssetCache, auto&& LoadFunc)
    {
        if (auto it = AssetCache.find(Path); it != AssetCache.end())
        {
            if (auto Existing = it->second)
            {
                //LOG("Cache hit for: {}", Path);
                return Existing;
            }
        }

        if (!AssetDirectory)
        {
            ERROR("No AssetDirectory for: {}", Path);
            return nullptr;
        }

        list<uint8> FileData;
        if (!AssetDirectory->ReadFile(Path, FileData))
        {
            ERROR("Failed to read: {}", Path);
            return nullptr;
        }

        auto NewAsset = make_shared<Asset>();
        if (!LoadFunc(*NewAsset, FileData))
        {
            ERROR("Failed to load: {}", Path);
            return nullptr;
        }

        AssetCache[Path] = NewAsset;
        return NewAsset;
    }

    shared<texture> ResourceSubsystem::LoadTexture(const string& Path)
    {
        return Load<texture>(Path, Textures,
            [](texture& Tex, const list<uint8>& Data) {
                return Tex.loadFromMemory(Data.data(), Data.size());
            });
    }

    shared<font> ResourceSubsystem::LoadFont(const string& Path)
    {
        if (auto it = Fonts.find(Path); it != Fonts.end())
            if (auto Existing = it->second)
                return Existing;

        auto DataIt = FontData.find(Path);
        if (DataIt == FontData.end())
        {
            if (!AssetDirectory) return nullptr;

            list<uint8> FileData;
            if (!AssetDirectory->ReadFile(Path, FileData)) return nullptr;

            list<uint8> Buffer(FileData.begin(), FileData.end());
            DataIt = FontData.emplace(Path, std::move(Buffer)).first;
        }

        auto NewFont = make_shared<font>();
        if (!NewFont->openFromMemory(DataIt->second.data(), DataIt->second.size()))
        {
            ERROR("Failed to load font: {}", Path);
            return nullptr;
        }

        Fonts[Path] = NewFont;
        return NewFont;
    }

    shared<soundBuffer> ResourceSubsystem::LoadSound(const string& Path)
    {
        return Load<soundBuffer>(Path, Sounds,
            [](soundBuffer& Snd, const list<uint8>& Data) {
                return Snd.loadFromMemory(Data.data(), Data.size());
            });
    }

    shared<sf::Music> ResourceSubsystem::LoadMusic(const string& Path)
    {
        auto It = MusicData.find(Path);
        if (It == MusicData.end())
        {
            if (!AssetDirectory)
            {
                ERROR("No AssetDirectory for: {}", Path);
                return nullptr;
            }

            list<uint8> FileData;
            if (!AssetDirectory->ReadFile(Path, FileData))
            {
                ERROR("Failed to read music: {}", Path);
                return nullptr;
            }

            auto DataPtr = make_shared<list<uint8>>(std::move(FileData));
            It = MusicData.emplace(Path, DataPtr).first;
        }

        auto Stream = make_shared<MusicMemoryStream>(It->second);
        auto Music = make_shared<sf::Music>();

        if (!Music->openFromStream(*Stream))
        {
            ERROR("Failed to open music from stream: {}", Path);
            return nullptr;
        }

        MusicStreams[Path] = Stream;
        return Music;
    }

    shared<shader> ResourceSubsystem::LoadShader(const string& Path, shader::Type Type)
    {
        if (!AssetDirectory) return nullptr;

        list<uint8> Data;
        if (!AssetDirectory->ReadFile(Path, Data)) return nullptr;

        string Source(Data.begin(), Data.end());
        auto Shader = make_shared<shader>();
        if (!Shader->loadFromMemory(Source, Type)) return nullptr;

        return Shader;
    }
}
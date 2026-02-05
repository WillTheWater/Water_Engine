// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
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
        Music.clear();
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
                LOG("Cache hit for: {}", Path);
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

    string ResourceSubsystem::StreamMusic(const string& Path, const list<uint8>& Data)
    {
        auto FileName = Path.substr(Path.find_last_of("/\\") + 1);
        auto TempPath = std::filesystem::temp_directory_path() / FileName;

        int Counter = 0;
        while (std::filesystem::exists(TempPath))
        {
            TempPath = std::filesystem::temp_directory_path() / (std::to_string(Counter++) + "_" + FileName);
        }

        std::ofstream OutFile(TempPath, std::ios::binary);
        OutFile.write(reinterpret_cast<const char*>(Data.data()), Data.size());

        return TempPath.string();
    }

    shared<sf::Music> ResourceSubsystem::LoadMusic(const string& Path)
    {
        string TargetTempPath;
        auto it = Music.find(Path);
        if (it != Music.end())
        {
            TargetTempPath = it->second;
        }
        else
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

            TargetTempPath = StreamMusic(Path, FileData);
            Music[Path] = TargetTempPath;
        }

        auto MusicInstance = make_shared<sf::Music>();
        if (!MusicInstance->openFromFile(TargetTempPath))
        {
            ERROR("Failed to open music file: {}", TargetTempPath);
            return nullptr;
        }

        return MusicInstance;
    }
}
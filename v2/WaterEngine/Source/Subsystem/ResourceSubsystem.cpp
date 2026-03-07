// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

#ifdef USE_PACKED_ASSETS
#include <physfs.h>
#endif

namespace we
{
    ResourceSubsystem* ResourceSubsystem::Instance = nullptr;

    ResourceSubsystem::ResourceSubsystem()
    {
        Instance = this;
        
        #ifdef USE_PACKED_ASSETS
            if (!PHYSFS_init(nullptr))
            {
                ERROR("ResourceSubsystem: Failed to initialize PhysFS");
                return;
            }
            
            if (!PHYSFS_mount(ASSET_PACK_PATH, "/", 1))
            {
                ERROR("ResourceSubsystem: Failed to mount {}", ASSET_PACK_PATH);
                return;
            }
            
            LOG("ResourceSubsystem: Mounted {}", ASSET_PACK_PATH);
        #endif
    }

    ResourceSubsystem::~ResourceSubsystem()
    {
        #ifdef USE_PACKED_ASSETS
            PHYSFS_deinit();
        #endif
    }

    string ResourceSubsystem::ResolvePath(const string& Filename)
    {
        #ifdef USE_RAW_ASSETS
            return ASSET_ROOT_PATH + Filename;
        #else
            // In Release, paths are relative to pak root (Assets/... not Content/Assets/...)
            return Filename;
        #endif
    }

    #ifdef USE_PACKED_ASSETS
    string ResourceSubsystem::LoadFileData(const string& Path)
    {
        PHYSFS_File* File = PHYSFS_openRead(Path.c_str());
        if (!File)
        {
            ERROR("ResourceSubsystem: Failed to open file {}", Path);
            return "";
        }
        
        PHYSFS_sint64 Length = PHYSFS_fileLength(File);
        if (Length < 0)
        {
            PHYSFS_close(File);
            return "";
        }
        
        string FileData;
        FileData.resize(static_cast<size_t>(Length));
        
        PHYSFS_readBytes(File, FileData.data(), static_cast<PHYSFS_uint64>(Length));
        PHYSFS_close(File);
        
        return FileData;
    }
    #endif

    shared<texture> ResourceSubsystem::LoadTexture(const string& Filename)
    {
        if (Textures.contains(Filename))
            return Textures[Filename];

        string Path = ResolvePath(Filename);
        auto Tex = make_shared<texture>();
        
        #ifdef USE_RAW_ASSETS
            if (!Tex->loadFromFile(Path))
            {
                ERROR("ResourceSubsystem: Failed to load texture {}", Filename);
                return nullptr;
            }
        #else
            string FileData = LoadFileData(Path);
            if (FileData.empty() || !Tex->loadFromMemory(
                reinterpret_cast<const uint8_t*>(FileData.data()), FileData.size()))
            {
                ERROR("ResourceSubsystem: Failed to load texture {}", Filename);
                return nullptr;
            }
            Data[Filename] = std::move(FileData);
        #endif

        Textures[Filename] = Tex;
        return Tex;
    }

    shared<soundBuffer> ResourceSubsystem::LoadSound(const string& Filename)
    {
        if (Sounds.contains(Filename))
            return Sounds[Filename];

        string Path = ResolvePath(Filename);
        auto Snd = make_shared<soundBuffer>();
        
        #ifdef USE_RAW_ASSETS
            if (!Snd->loadFromFile(Path))
            {
                ERROR("ResourceSubsystem: Failed to load sound {}", Filename);
                return nullptr;
            }
        #else
            string FileData = LoadFileData(Path);
            if (FileData.empty() || !Snd->loadFromMemory(
                reinterpret_cast<const uint8_t*>(FileData.data()), FileData.size()))
            {
                ERROR("ResourceSubsystem: Failed to load sound {}", Filename);
                return nullptr;
            }
            Data[Filename] = std::move(FileData);
        #endif

        Sounds[Filename] = Snd;
        return Snd;
    }

    shared<font> ResourceSubsystem::LoadFont(const string& Filename)
    {
        if (Fonts.contains(Filename))
            return Fonts[Filename];

        string Path = ResolvePath(Filename);
        auto Fnt = make_shared<font>();
        
        #ifdef USE_RAW_ASSETS
            if (!Fnt->openFromFile(Path))
            {
                ERROR("ResourceSubsystem: Failed to load font {}", Filename);
                return nullptr;
            }
        #else
            string FileData = LoadFileData(Path);
            if (FileData.empty() || !Fnt->openFromMemory(
                reinterpret_cast<const uint8_t*>(FileData.data()), FileData.size()))
            {
                ERROR("ResourceSubsystem: Failed to load font {}", Filename);
                return nullptr;
            }
            Data[Filename] = std::move(FileData);
        #endif

        Fonts[Filename] = Fnt;
        return Fnt;
    }

    shared<music> ResourceSubsystem::LoadMusic(const string& Filename)
    {
        if (Music.contains(Filename))
            return Music[Filename];

        string Path = ResolvePath(Filename);
        auto Mus = make_shared<music>();
        
        #ifdef USE_RAW_ASSETS
            if (!Mus->openFromFile(Path))
            {
                ERROR("ResourceSubsystem: Failed to load music {}", Filename);
                return nullptr;
            }
        #else
            string FileData = LoadFileData(Path);
            if (FileData.empty() || !Mus->openFromMemory(
                reinterpret_cast<const uint8_t*>(FileData.data()), FileData.size()))
            {
                ERROR("ResourceSubsystem: Failed to load music {}", Filename);
                return nullptr;
            }
            Data[Filename] = std::move(FileData);
        #endif

        Music[Filename] = Mus;
        return Mus;
    }

    ResourceSubsystem& ResourceSubsystem::Get()
    {
        return *Instance;
    }

    template<typename MapType>
    void ResourceSubsystem::CleanCache(MapType& Cache, const string& TypeName)
    {
        for (auto it = Cache.begin(); it != Cache.end();)
        {
            if (it->second.use_count() == 1)
            {
                LOG("ResourceSubsystem GC: Unloading {} '{}'", TypeName, it->first);
                it = Cache.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void ResourceSubsystem::GarbageCollect()
    {
        CleanCache(Textures, "texture");
        CleanCache(Sounds, "sound");
        CleanCache(Fonts, "font");
        CleanCache(Music, "music");
    }
}
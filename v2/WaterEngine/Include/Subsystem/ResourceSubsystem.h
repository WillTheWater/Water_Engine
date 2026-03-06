// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Utility/Log.h"

namespace we
{
    class ResourceSubsystem
    {
    public:
        ResourceSubsystem();
        ~ResourceSubsystem();

        static ResourceSubsystem& Get();

        shared<texture>     LoadTexture(const string& Filename);
        shared<soundBuffer> LoadSound(const string& Filename);
        shared<font>        LoadFont(const string& Filename);
        shared<music>       LoadMusic(const string& Filename);

        void GarbageCollect();

    private:
        template<typename MapType>
        void CleanCache(MapType& Cache, const string& TypeName);

        string ResolvePath(const string& Filename);
        
        #ifdef USE_PACKED_ASSETS
        string LoadFileData(const string& Path);
        #endif

    private:
        dictionary<string, shared<texture>>     Textures;
        dictionary<string, shared<soundBuffer>> Sounds;
        dictionary<string, shared<font>>        Fonts;
        dictionary<string, shared<music>>       Music;
        dictionary<string, string>              Data;
    };

    inline ResourceSubsystem& LoadAsset() { return ResourceSubsystem::Get(); }
}
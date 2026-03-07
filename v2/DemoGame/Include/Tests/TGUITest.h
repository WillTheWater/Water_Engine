// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

// TGUI Backend - must be included to get tgui::Gui type
#include <TGUI/Backend/SFML-Graphics.hpp>

// PhysFS for .pak loading
#ifdef USE_PACKED_ASSETS
#include <physfs.h>
#endif

namespace we
{
    // Helper to load files via PhysFS for TGUI
    class PhysFSHelper
    {
    public:
        // Load file from PhysFS into memory buffer
        static std::vector<std::uint8_t> LoadFile(const std::string& path);
        
        // Check if running with packed assets
        static bool IsUsingPhysFS();
    };

    // Test TGUI integration with PhysFS/raw asset loading
    class TGUITest
    {
    public:
        TGUITest();
        ~TGUITest();

        // Run all tests, returns true if all passed
        bool RunTests();

    private:
        bool TestFontLoading();
        bool TestTextureLoading();
        bool TestWidgetCreation(tgui::Gui& gui);
        
        // PhysFS-enabled loaders
        tgui::Font LoadFontFromPhysFS(const std::string& path);
        tgui::Texture LoadTextureFromPhysFS(const std::string& path);

    private:
        bool bFontLoaded = false;
        bool bTextureLoaded = false;
        bool bWidgetCreated = false;
    };
}

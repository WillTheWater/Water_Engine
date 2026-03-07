// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Tests/TGUITest.h"
#include "Utility/Log.h"
#include "Subsystem/ResourceSubsystem.h"

#include <fstream>  // For PhysFSHelper raw file loading

// TGUI Backend includes
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>

namespace we
{
    // =============================================================================
    // PhysFS Helper Implementation
    // =============================================================================
    
    bool PhysFSHelper::IsUsingPhysFS()
    {
#ifdef USE_PACKED_ASSETS
        return true;
#else
        return false;
#endif
    }
    
    std::vector<std::uint8_t> PhysFSHelper::LoadFile(const std::string& path)
    {
        std::vector<std::uint8_t> buffer;
        
#ifdef USE_PACKED_ASSETS
        // Debug: Check if PhysFS is initialized
        if (!PHYSFS_isInit())
        {
            LOG("[PhysFS] ERROR: PhysFS not initialized!");
            return buffer;
        }
        
        // Debug: List search path
        LOG("[PhysFS] Attempting to open: {}", path);
        LOG("[PhysFS] Base dir: {}", PHYSFS_getBaseDir());
        LOG("[PhysFS] Write dir: {}", PHYSFS_getWriteDir() ? PHYSFS_getWriteDir() : "(none)");
        
        char** searchPath = PHYSFS_getSearchPath();
        if (searchPath)
        {
            for (int i = 0; searchPath[i] != nullptr; i++)
            {
                LOG("[PhysFS] Search path[{}]: {}", i, searchPath[i]);
            }
        }
        
        PHYSFS_File* file = PHYSFS_openRead(path.c_str());
        if (!file)
        {
            LOG("[PhysFS] Failed to open: {} - Error: {}", path, PHYSFS_getLastError());
            return buffer;
        }
        
        PHYSFS_sint64 length = PHYSFS_fileLength(file);
        if (length > 0)
        {
            buffer.resize(static_cast<size_t>(length));
            PHYSFS_readBytes(file, buffer.data(), static_cast<PHYSFS_uint64>(length));
        }
        
        PHYSFS_close(file);
#else
        // Raw file loading for Debug mode
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (file.is_open())
        {
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            buffer.resize(size);
            file.read(reinterpret_cast<char*>(buffer.data()), size);
        }
#endif
        return buffer;
    }

    // =============================================================================
    // TGUITest Implementation
    // =============================================================================
    
    TGUITest::TGUITest() = default;
    TGUITest::~TGUITest() = default;

    bool TGUITest::RunTests()
    {
        LOG("=== TGUI TEST START ===");
        LOG("[TGUI Test] PhysFS Mode: {}", PhysFSHelper::IsUsingPhysFS() ? "YES (pak file)" : "NO (raw files)");
        
#ifdef USE_PACKED_ASSETS
        // Ensure PhysFS is initialized (trigger singleton creation)
        LoadAsset().GarbageCollect();  // Just to initialize the singleton
#endif
        
        try
        {
            // Create a dummy render texture for TGUI backend initialization
            sf::RenderTexture dummyTarget;
            if (!dummyTarget.resize({100, 100}))
            {
                LOG("[TGUI Test] Failed to create dummy render target");
                return false;
            }
            
            // Create Gui object attached to the target
            tgui::Gui gui(dummyTarget);
            LOG("[TGUI Test] TGUI Backend initialized successfully");
            
            bFontLoaded = TestFontLoading();
            bTextureLoaded = TestTextureLoading();
            bWidgetCreated = TestWidgetCreation(gui);
        }
        catch (const std::exception& e)
        {
            LOG("[TGUI Test] Exception during initialization: {}", e.what());
            return false;
        }
        
        bool bAllPassed = bFontLoaded && bTextureLoaded && bWidgetCreated;
        
        LOG("=== TGUI TEST RESULTS ===");
        LOG("Font Loading:    {}", bFontLoaded ? "PASS" : "FAIL");
        LOG("Texture Loading: {}", bTextureLoaded ? "PASS" : "FAIL");
        LOG("Widget Creation: {}", bWidgetCreated ? "PASS" : "FAIL");
        LOG("Overall:         {}", bAllPassed ? "ALL PASSED" : "SOME FAILED");
        
        return bAllPassed;
    }

    tgui::Font TGUITest::LoadFontFromPhysFS(const std::string& path)
    {
        auto buffer = PhysFSHelper::LoadFile(path);
        if (buffer.empty())
        {
            throw std::runtime_error("Failed to load font from PhysFS: " + path);
        }
        
        LOG("[TGUI Test] Loaded {} bytes from PhysFS for font", buffer.size());
        
        // Create font from memory buffer
        return tgui::Font(buffer.data(), buffer.size());
    }
    
    tgui::Texture TGUITest::LoadTextureFromPhysFS(const std::string& path)
    {
        auto buffer = PhysFSHelper::LoadFile(path);
        if (buffer.empty())
        {
            throw std::runtime_error("Failed to load texture from PhysFS: " + path);
        }
        
        LOG("[TGUI Test] Loaded {} bytes from PhysFS for texture", buffer.size());
        
        // Create texture and load from memory
        tgui::Texture texture;
        texture.loadFromMemory(buffer.data(), buffer.size());
        return texture;
    }

    bool TGUITest::TestFontLoading()
    {
        std::string fontPath = "Assets/Font/Default/defaultFont.otf";
        LOG("[TGUI Test] Loading font from: {}", fontPath);
        
        try
        {
            tgui::Font font;
            
#ifdef USE_PACKED_ASSETS
            // Use PhysFS loading
            font = LoadFontFromPhysFS(fontPath);
#else
            // Use raw file path
            std::string fullPath = std::string(ASSET_ROOT_PATH) + fontPath;
            font = tgui::Font(fullPath);
#endif
            
            LOG("[TGUI Test] Font loaded successfully!");
            return true;
        }
        catch (const std::exception& e)
        {
            LOG("[TGUI Test] Font loading exception: {}", e.what());
            return false;
        }
    }

    bool TGUITest::TestTextureLoading()
    {
        std::string texturePath = "Assets/Icon/icon.png";
        LOG("[TGUI Test] Loading texture from: {}", texturePath);
        
        try
        {
            tgui::Texture texture;
            
#ifdef USE_PACKED_ASSETS
            // Use PhysFS loading
            texture = LoadTextureFromPhysFS(texturePath);
#else
            // Use raw file path
            std::string fullPath = std::string(ASSET_ROOT_PATH) + texturePath;
            texture = tgui::Texture(fullPath);
#endif
            
            if (texture.getData())
            {
                LOG("[TGUI Test] Texture loaded successfully! Size: {}x{}", 
                    texture.getImageSize().x, texture.getImageSize().y);
                return true;
            }
            else
            {
                LOG("[TGUI Test] Texture failed to load (null data)");
                return false;
            }
        }
        catch (const std::exception& e)
        {
            LOG("[TGUI Test] Texture loading exception: {}", e.what());
            return false;
        }
    }

    bool TGUITest::TestWidgetCreation(tgui::Gui& gui)
    {
        LOG("[TGUI Test] Creating TGUI widgets...");
        
        try
        {
            // Create a button
            auto button = tgui::Button::create("Test Button");
            if (!button)
            {
                LOG("[TGUI Test] Failed to create button");
                return false;
            }
            
            // Try to set a font (using PhysFS if in Release mode)
            try
            {
                tgui::Font font;
#ifdef USE_PACKED_ASSETS
                font = LoadFontFromPhysFS("Assets/Font/Default/defaultFont.otf");
#else
                std::string fontPath = std::string(ASSET_ROOT_PATH) + "Assets/Font/Default/defaultFont.otf";
                font = tgui::Font(fontPath);
#endif
                button->setInheritedFont(font);
                LOG("[TGUI Test] Button created with font");
            }
            catch (...)
            {
                LOG("[TGUI Test] Button created (font failed, using default)");
            }
            
            // Test setting position and size
            button->setPosition({50, 50});
            button->setSize({200, 40});
            
            LOG("[TGUI Test] Button position: ({}, {}), size: ({}, {})",
                button->getPosition().x, button->getPosition().y,
                button->getSize().x, button->getSize().y);
            
            // Add to gui
            gui.add(button);
            LOG("[TGUI Test] Button added to GUI");
            
            // Try to load a background texture
            try
            {
#ifdef USE_PACKED_ASSETS
                auto bgTexture = LoadTextureFromPhysFS("Assets/Textures/Default/defaultBackground.png");
#else
                std::string bgPath = std::string(ASSET_ROOT_PATH) + "Assets/Textures/Default/defaultBackground.png";
                tgui::Texture bgTexture(bgPath);
#endif
                LOG("[TGUI Test] Background texture available for theming");
            }
            catch (...)
            {
                LOG("[TGUI Test] Background texture failed (non-critical)");
            }
            
            return true;
        }
        catch (const std::exception& e)
        {
            LOG("[TGUI Test] Widget creation exception: {}", e.what());
            return false;
        }
    }
}

// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WindowSubsystem.h"
#include "Core/EngineConfig.h"
#include "Utility/Log.h"

namespace we
{
    WindowSubsystem::WindowSubsystem(const EngineConfig::WindowConfig& Config)
    {
        // Create window with config dimensions
        const sf::VideoMode Mode({ Config.Width, Config.Height });
        const sf::State State = Config.Fullscreen ? sf::State::Fullscreen : sf::State::Windowed;
        const uint Style = sf::Style::Default;

        create(Mode, Config.Title, Style, State);
        
        // Configure VSync / FPS limit
        // SFML: Use VSync OR FramerateLimit, not both
        ConfigureFrameLimit(Config);

        LOG("Window created: {}x{} | VSync: {} | TargetFPS: {}", 
            Config.Width, Config.Height, Config.VSync, Config.TargetFPS);
    }

    void WindowSubsystem::ConfigureFrameLimit(const EngineConfig::WindowConfig& Config)
    {
        if (Config.VSync)
        {
            // VSync enabled: let GPU sync to monitor, ignore TargetFPS
            setVerticalSyncEnabled(true);
            setFramerateLimit(0);  // Disable limiter when using VSync
        }
        else
        {
            // VSync disabled: use software frame limiter
            setVerticalSyncEnabled(false);
            setFramerateLimit(static_cast<unsigned int>(Config.TargetFPS));
        }
    }
}

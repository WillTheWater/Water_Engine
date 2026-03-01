// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WindowSubsystem.h"
#include "Core/EngineConfig.h"
#include "Utility/Log.h"

namespace we
{
    WindowSubsystem::WindowSubsystem(const EngineConfig::WindowConfig& InConfig)
        : bIsFullscreen{ InConfig.Fullscreen }
        , Config{ InConfig }
    {
        const sf::VideoMode Mode(Config.DefaultSize);
        const sf::State State = Config.Fullscreen ? sf::State::Fullscreen : sf::State::Windowed;
        const uint Style = sf::Style::Default;
        
        CreateGameWindow(Mode, Style, State);
        
        LOG("Window created: {}x{} | Fullscreen: {} | VSync: {} | TargetFPS: {}", 
            Config.DefaultSize.x, Config.DefaultSize.y, Config.Fullscreen, Config.VSync, Config.TargetFPS);
    }

    void WindowSubsystem::CreateGameWindow(const sf::VideoMode& Mode, uint Style, sf::State State)
    {
        create(Mode, Config.Title, Style, State);
        ApplyWindowSettings();
    }

    void WindowSubsystem::ApplyWindowSettings()
    {
        // Disable key repeat - InputSubsystem handles held keys via OnHeld callbacks
        setKeyRepeatEnabled(false);
        
        // Configure VSync / FPS limit
        ConfigureFrameLimit(Config);
    }

    void WindowSubsystem::ConfigureFrameLimit(const EngineConfig::WindowConfig& Config)
    {
        if (Config.VSync)
        {
            setVerticalSyncEnabled(true);
            setFramerateLimit(0);
        }
        else
        {
            setVerticalSyncEnabled(false);
            setFramerateLimit(static_cast<unsigned int>(Config.TargetFPS));
        }
    }

    // =========================================================================
    // Resize Handling - Enforces aspect ratio and minimum window size
    // =========================================================================
    void WindowSubsystem::onResize()
    {
        // Skip aspect ratio enforcement in fullscreen mode
        if (bIsFullscreen) return;
        
        // Prevent recursive resize events when we call setSize()
        if (bIsResizing) return;

        vec2u NewSize = getSize();

        // Apply minimum size constraints
        NewSize.x = std::max(NewSize.x, Config.MinimumSize.x);
        NewSize.y = std::max(NewSize.y, Config.MinimumSize.y);

        // Apply aspect ratio constraint (derived from DefaultSize)
        NewSize = CalculateAspectRatioSize(NewSize);

        // Apply the corrected size if it changed
        if (NewSize != getSize())
        {
            bIsResizing = true;
            setSize(NewSize);
            bIsResizing = false;
        }
    }

    vec2u WindowSubsystem::CalculateAspectRatioSize(const vec2u& Size) const
    {
        // Calculate target aspect ratio from DefaultSize
        const float TargetRatio = static_cast<float>(Config.DefaultSize.x) / Config.DefaultSize.y;
        const float CurrentRatio = static_cast<float>(Size.x) / Size.y;

        // Already at target ratio - no adjustment needed
        if (std::abs(CurrentRatio - TargetRatio) <= 0.001f)
        {
            return Size;
        }

        vec2u AdjustedSize = Size;
        
        // Window is too wide - adjust width to match target ratio
        if (CurrentRatio > TargetRatio)
        {
            AdjustedSize.x = static_cast<uint>(Size.y * TargetRatio);
        }
        // Window is too tall - adjust height to match target ratio
        else
        {
            AdjustedSize.y = static_cast<uint>(Size.x / TargetRatio);
        }
        
        return AdjustedSize;
    }

    void WindowSubsystem::HandleEvent(const sf::Event& Event)
    {
        WindowEventHandler Handler{ *this };
        Event.visit(Handler);
    }

    // =========================================================================
    // Fullscreen Toggle
    // =========================================================================
    void WindowSubsystem::SetFullscreen(bool bFullscreen)
    {
        if (bIsFullscreen == bFullscreen) return;
        EventToggleBorderlessFullscreen();
    }

    void WindowSubsystem::EventToggleBorderlessFullscreen()
    {
        bIsFullscreen = !bIsFullscreen;

        if (bIsFullscreen)
        {
            CreateGameWindow(sf::VideoMode::getDesktopMode(), sf::Style::None, sf::State::Fullscreen);
        }
        else
        {
            CreateGameWindow(
                sf::VideoMode(Config.DefaultSize), 
                sf::Style::Default, 
                sf::State::Windowed
            );
            
            const auto Desktop = sf::VideoMode::getDesktopMode().size;
            const vec2i CenterPos(
                (Desktop.x - Config.DefaultSize.x) / 2, 
                (Desktop.y - Config.DefaultSize.y) / 2
            );
            setPosition(CenterPos);
        }
    }

    // =========================================================================
    // VSync Toggle
    // =========================================================================
    void WindowSubsystem::SetVSync(bool bEnabled)
    {
        Config.VSync = bEnabled;
        
        if (bEnabled)
        {
            setVerticalSyncEnabled(true);
            setFramerateLimit(0);
        }
        else
        {
            setVerticalSyncEnabled(false);
            setFramerateLimit(Config.TargetFPS);
        }
        
        LOG("VSync {} | TargetFPS: {}", bEnabled ? "enabled" : "disabled", Config.TargetFPS);
    }

    // =========================================================================
    // Mouse Position
    // =========================================================================
    vec2f WindowSubsystem::GetMousePosition() const
    {
        return vec2f(sf::Mouse::getPosition(*this));
    }
}

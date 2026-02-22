// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EngineConfig.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    // =========================================================================
    // Construction & Initialization
    // =========================================================================
    WindowSubsystem::WindowSubsystem(const WindowConfig& InConfig)
        : bIsFullscreen{ InConfig.FullscreenMode }
        , Config{ InConfig }
    {
        const sf::VideoMode mode(static_cast<vec2u>(Config.RenderResolution));
        const auto style = Config.FullscreenMode ? sf::Style::None : sf::Style::Default;
        const auto state = Config.FullscreenMode ? sf::State::Fullscreen : sf::State::Windowed;
        
        CreateGameWindow(mode, style, state);
        SetWindowIcon();
    }

    // =========================================================================
    // Event Handling
    // =========================================================================
    void WindowSubsystem::HandleEvent(const sf::Event& Event)
    {
        GameWindowEventHandler Handler{ *this };
        Event.visit(Handler);
    }

    // =========================================================================
    // Window Management
    // =========================================================================
    void WindowSubsystem::SetFullscreen(bool bFullscreen)
    {
        if (bIsFullscreen == bFullscreen) return;
        EventToggleBorderlessFullscreen();
    }

    void WindowSubsystem::SetVSync(bool bEnabled)
    {
        Config.VsyncEnabled = bEnabled;
        setVerticalSyncEnabled(bEnabled);
        
        if (!bEnabled)
        {
            setFramerateLimit(static_cast<uint>(Config.TargetFPS));
        }
        else
        {
            setFramerateLimit(0);
        }
    }

    vec2f WindowSubsystem::GetMousePosition() const
    {
        return vec2f(sf::Mouse::getPosition(*this));
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
        NewSize.x = std::max(NewSize.x, static_cast<uint>(Config.WindowMinimumSize.x));
        NewSize.y = std::max(NewSize.y, static_cast<uint>(Config.WindowMinimumSize.y));

        // Apply aspect ratio constraint
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
        const float TargetRatio = Config.AspectRatio.x / Config.AspectRatio.y;
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

    // =========================================================================
    // Window Creation & Settings
    // =========================================================================
    void WindowSubsystem::CreateGameWindow(const sf::VideoMode& Mode, uint Style, sf::State State)
    {
        create(Mode, std::string(Config.WindowName), Style, State);
        ApplyWindowSettings();
    }

    void WindowSubsystem::ApplyWindowSettings()
    {
        SetWindowIcon();
        setKeyRepeatEnabled(Config.EnableKeyRepeat);
        setMouseCursorVisible(false);

        if (Config.VsyncEnabled) 
        { 
            setVerticalSyncEnabled(Config.VsyncEnabled); 
        }
        else 
        { 
            setFramerateLimit(static_cast<uint>(Config.TargetFPS)); 
        }
    }

    void WindowSubsystem::SetWindowIcon()
    {
        auto IconHandle = LoadAsset().LoadTextureSync(std::string(Config.WindowIcon));
        const auto& Image = IconHandle->copyToImage();
        setIcon(Image);
    }

    // =========================================================================
    // Event Handlers
    // =========================================================================
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
                sf::VideoMode(static_cast<sf::Vector2u>(Config.RenderResolution)), 
                sf::Style::Default, 
                sf::State::Windowed
            );
            
            const auto Desktop = sf::VideoMode::getDesktopMode().size;
            const vec2i CenterPos(
                (Desktop.x - Config.RenderResolution.x) / 2, 
                (Desktop.y - Config.RenderResolution.y) / 2
            );
            setPosition(CenterPos);
        }
    }

    void WindowSubsystem::EventWindowClose()
    {
        close();
    }

} // namespace we

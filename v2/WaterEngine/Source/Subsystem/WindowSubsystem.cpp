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

    void WindowSubsystem::HandleEvent(const sf::Event& Event)
    {
        GameWindowEventHandler Handler{ *this };
        Event.visit(Handler);
    }

    vec2f WindowSubsystem::GetMousePosition() const
    {
        return vec2f(sf::Mouse::getPosition(*this));
    }

    void WindowSubsystem::onResize()
    {
        // 1. Don't enforce size in fullscreen
        if (bIsFullscreen) return;

        // 2. Prevent Infinite Recursion
        if (bIsResizing) return;

        vec2u NewSize = getSize();

        // 3. Enforce Minimum Size
        if (NewSize.x < Config.WindowMinimumSize.x) NewSize.x = Config.WindowMinimumSize.x;
        if (NewSize.y < Config.WindowMinimumSize.y) NewSize.y = Config.WindowMinimumSize.y;

        // 4. Enforce Aspect Ratio
        const float TargetRatio = Config.AspectRatio.x / Config.AspectRatio.y;
        const float CurrentRatio = static_cast<float>(NewSize.x) / NewSize.y;

        if (std::abs(CurrentRatio - TargetRatio) > 0.001f)
        {
            if (CurrentRatio > TargetRatio)
                NewSize.x = static_cast<uint>(NewSize.y * TargetRatio);
            else
                NewSize.y = static_cast<uint>(NewSize.x / TargetRatio);
        }

        // 5. Apply corrected size if changed
        if (NewSize != getSize())
        {
            bIsResizing = true;
            setSize(NewSize);
            bIsResizing = false;
        }
    }

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

        if (Config.VsyncEnabled) { setVerticalSyncEnabled(Config.VsyncEnabled); }
        else { setFramerateLimit(static_cast<uint>(Config.TargetFPS)); }
    }

    void WindowSubsystem::SetWindowIcon()
    {
        auto IconHandle = LoadAsset().LoadTextureSync(std::string(Config.WindowIcon));
        const auto& Image = IconHandle->copyToImage();
        setIcon(Image);
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
            CreateGameWindow(sf::VideoMode(static_cast<sf::Vector2u>(Config.RenderResolution)), sf::Style::Default, sf::State::Windowed);
            const auto desktop = sf::VideoMode::getDesktopMode().size;
            setPosition(vec2i((desktop.x - Config.RenderResolution.x) / 2, (desktop.y - Config.RenderResolution.y) / 2));
        }
    }

    void WindowSubsystem::EventWindowClose()
    {
        close();
    }

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


}
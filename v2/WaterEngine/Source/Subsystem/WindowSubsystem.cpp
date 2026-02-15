// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WindowSubsystem.h"
#include "EngineConfig.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
    WindowSubsystem::WindowSubsystem()
        : WindowedPosition{ vec2i(sf::VideoMode::getDesktopMode().size.x / 4, sf::VideoMode::getDesktopMode().size.y / 4) }
        , bIsFullscreen{ EC.FullscreenMode }
    {
        const sf::VideoMode mode(static_cast<vec2u>(EC.WindowSize));
        const auto style = EC.FullscreenMode ? sf::Style::None : sf::Style::Default;
        const auto state = EC.FullscreenMode ? sf::State::Fullscreen : sf::State::Windowed;
        CreateGameWindow(mode, style, state);
    }

    void WindowSubsystem::onResize()
    {
        const vec2f TargetRes = vec2f(EC.AspectRatio);
        const float TargetRatio = TargetRes.x / TargetRes.y;

        vec2u NewSize = getSize();
        NewSize.x = std::max(NewSize.x, static_cast<uint>(EC.WindowMinimumSize.x));
        NewSize.y = std::max(NewSize.y, static_cast<uint>(EC.WindowMinimumSize.y));

        float currentRatio = static_cast<float>(NewSize.x) / NewSize.y;

        if (std::abs(currentRatio - TargetRatio) > 0.001f)
        {
            if (currentRatio > TargetRatio)
                NewSize.x = static_cast<uint>(NewSize.y * TargetRatio);
            else
                NewSize.y = static_cast<uint>(NewSize.x / TargetRatio);
        }

        if (NewSize != getSize())
        {
            setSize(NewSize);
        }
    }

    void WindowSubsystem::CreateGameWindow(const sf::VideoMode& Mode, uint Style, sf::State State)
    {
        create(Mode, EC.WindowName, Style, State);
        ApplyWindowSettings();
    }

    void WindowSubsystem::ApplyWindowSettings()
    {
        SetWindowIcon();
        setKeyRepeatEnabled(EC.EnableKeyRepeat);
        setMouseCursorVisible(false);

        // HVSync takes priority over FPS
        if (EC.VsyncEnabled)
        {
            setVerticalSyncEnabled(EC.VsyncEnabled);
        }
        else
        {
            setFramerateLimit(static_cast<uint>(EC.TargetFPS));
        }
    }

    void WindowSubsystem::SetWindowIcon()
    {
       // auto IconTexture = Asset().LoadTexture(EC.WindowIcon);
        //const auto& Image = IconTexture->copyToImage();
        //setIcon(Image);
    }

    void WindowSubsystem::EventToggleBorderlessFullscreen()
    {
        bIsFullscreen = !bIsFullscreen;

        if (bIsFullscreen)
        {
            WindowedPosition = getPosition();
            const auto Desktop = sf::VideoMode::getDesktopMode();
            CreateGameWindow(Desktop, sf::Style::None, sf::State::Fullscreen);
        }
        else
        {
            const sf::VideoMode Mode(static_cast<sf::Vector2u>(EC.WindowSize));
            CreateGameWindow(Mode, sf::Style::Default, sf::State::Windowed);
            setPosition(WindowedPosition);
        }
        EventWindowResized();
    }

    view WindowSubsystem::getConstrainedView() const
    {
        float targetRatio = static_cast<float>(EC.AspectRatio.x) / static_cast<float>(EC.AspectRatio.y);
        vec2u WinSize = getSize();
        float windowRatio = static_cast<float>(WinSize.x) / static_cast<float>(WinSize.y);

        float vWidth = 1.0f, vHeight = 1.0f, vPosX = 0.0f, vPosY = 0.0f;

        if (windowRatio > targetRatio) {
            vWidth = targetRatio / windowRatio;
            vPosX = (1.0f - vWidth) / 2.0f;
        }
        else {
            vHeight = windowRatio / targetRatio;
            vPosY = (1.0f - vHeight) / 2.0f;
        }

        view ConstrainedView;
        ConstrainedView.setSize(vec2f(EC.WindowSize));
        ConstrainedView.setCenter(vec2f(EC.WindowSize) / 2.0f);
        ConstrainedView.setViewport(rectf({ vPosX, vPosY }, { vWidth, vHeight }));

        return ConstrainedView;
    }

    void WindowSubsystem::EventWindowClose()
    {
        close();
    }

    void WindowSubsystem::EventWindowResized()
    {
    }

    void WindowSubsystem::EventWindowFocusLost()
    {
    }

    void WindowSubsystem::EventWindowFocusGained()
    {
    }

    void WindowSubsystem::EventJoypadConnected()
    {
    }

    void WindowSubsystem::EventJoypadDisconnected()
    {
    }

    void WindowSubsystem::EventJoypadButtonPressed()
    {
    }

    void WindowSubsystem::EventJoypadButtonReleased()
    {
    }

    void WindowSubsystem::EventKeyPressed()
    {
    }

    void WindowSubsystem::EventKeyReleased()
    {
    }

    void WindowSubsystem::EventMouseButtonPressed()
    {
    }

    void WindowSubsystem::EventMouseButtonReleased()
    {
    }
}
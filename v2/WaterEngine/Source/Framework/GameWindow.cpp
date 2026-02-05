// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/GameWindow.h"
#include "EngineConfig.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
    GameWindow::GameWindow()
        : WindowedPosition{ vec2i(sf::VideoMode::getDesktopMode().size.x / 4, sf::VideoMode::getDesktopMode().size.y / 4) }
        , bIsFullscreen{ EC.FullscreenMode }
    {
        const sf::VideoMode mode(static_cast<vec2u>(EC.WindowSize));
        const auto style = EC.FullscreenMode ? sf::Style::None : sf::Style::Default;
        const auto state = EC.FullscreenMode ? sf::State::Fullscreen : sf::State::Windowed;
        CreateGameWindow(mode, style, state);
    }

    void GameWindow::onResize()
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

    void GameWindow::CreateGameWindow(const sf::VideoMode& Mode, uint Style, sf::State State)
    {
        create(Mode, EC.WindowName, Style, State);
        ApplyWindowSettings();
    }

    void GameWindow::ApplyWindowSettings()
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

    void GameWindow::SetWindowIcon()
    {
        auto IconTexture = Asset().LoadTexture(EC.WindowIcon);
        const auto& Image = IconTexture->copyToImage();
        setIcon(Image);
    }

    void GameWindow::EventToggleBorderlessFullscreen()
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

    void GameWindow::EventWindowClose()
    {
        close();
    }

    void GameWindow::EventWindowResized()
    {
        OnResize.Broadcast(getSize());
    }

    void GameWindow::EventWindowFocusLost()
    {
    }

    void GameWindow::EventWindowFocusGained()
    {
    }

    void GameWindow::EventJoypadConnected()
    {
    }

    void GameWindow::EventJoypadDisconnected()
    {
    }

    void GameWindow::EventJoypadButtonPressed()
    {
    }

    void GameWindow::EventJoypadButtonReleased()
    {
    }

    void GameWindow::EventKeyPressed()
    {
    }

    void GameWindow::EventKeyReleased()
    {
    }

    void GameWindow::EventMouseButtonPressed()
    {
    }

    void GameWindow::EventMouseButtonReleased()
    {
    }
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/GameWindow.h"
#include "EngineConfig.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    GameWindow::GameWindow()
        : WindowedPosition{ vec2i(sf::VideoMode::getDesktopMode().size.x / 4, sf::VideoMode::getDesktopMode().size.y / 4) }
        , bIsFullscreen{ EC.FullscreenMode }
        , MinimumSize{ EC.WindowMinimumSize }
        , AspectRatio{ EC.AspectRatio }
    {
        const sf::VideoMode mode(static_cast<sf::Vector2u>(EC.WindowSize));
        const auto style = EC.FullscreenMode ? sf::Style::None : sf::Style::Default;
        const auto state = EC.FullscreenMode ? sf::State::Fullscreen : sf::State::Windowed;
        CreateGameWindow(mode, style, state);
    }

    void GameWindow::onResize()
    {
        vec2u CurrentSize = getSize();

        // Enforce minimum Size
        if (CurrentSize.x < MinimumSize.x || CurrentSize.y < MinimumSize.y)
        {
            CurrentSize = vec2u(MinimumSize);
            setSize(CurrentSize);
        }

        // Enforce aspect ratio
        float targetRatio = AspectRatio.x / AspectRatio.y;
        float currentRatio = static_cast<float>(CurrentSize.x) / static_cast<float>(CurrentSize.y);

        if (std::abs(currentRatio - targetRatio) > 0.01f)
        {
            if (currentRatio > targetRatio) 
            {
                CurrentSize.x = static_cast<uint32_t>(CurrentSize.y * targetRatio);
            }
            else 
            {
                CurrentSize.y = static_cast<uint32_t>(CurrentSize.x / targetRatio);
            }
            setSize(CurrentSize);
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
            setFramerateLimit(static_cast<unsigned int>(EC.TargetFPS));
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
        OnResize(getSize());
    }

    void GameWindow::EventWindowClose()
    {
        close();
    }

    void GameWindow::EventWindowResized()
    {
        OnResize(getSize());
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
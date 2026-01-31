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
        , bIsFullscreen{EC.FullscreenMode }
    {
        // Initial creation
        const sf::VideoMode mode(static_cast<sf::Vector2u>(EC.WindowSize));
        const auto style = EC.FullscreenMode ? sf::Style::None : sf::Style::Default;
        const auto state = EC.FullscreenMode ? sf::State::Fullscreen : sf::State::Windowed;

        CreateGameWindow(mode, style, state);
    }

    void GameWindow::CreateGameWindow(const sf::VideoMode& Mode, uint Style, sf::State State)
    {
        create(Mode, EC.WindowName, Style, State);
        ApplyWindowSettings();

        if (OnResize)
            OnResize(getSize());
    }

    void GameWindow::ApplyWindowSettings()
    {
        SetWindowIcon();
        setKeyRepeatEnabled(EC.EnableKeyRepeat);

        // Handle mutual exclusion: VSync takes priority over FPS limit
        if (EC.VsyncEnabled)
        {
            setVerticalSyncEnabled(EC.VsyncEnabled);
            setFramerateLimit(0); // Disable FPS limit when VSync is on
        }
        else
        {
            setVerticalSyncEnabled(EC.VsyncEnabled);
            setFramerateLimit(static_cast<unsigned int>(EC.TargetFPS));
        }
    }

    void GameWindow::SetWindowIcon()
    {
        auto IconTexture = Asset().LoadTexture(EC.WindowIcon);
        const auto& image = IconTexture->copyToImage();
        setIcon(image);
    }

    void GameWindow::EventToggleBorderlessFullscreen()
    {
        bIsFullscreen = !bIsFullscreen;

        if (bIsFullscreen)
        {
            // Store windowed position before switching
            WindowedPosition = getPosition();

            // Create fullscreen borderless
            const auto desktop = sf::VideoMode::getDesktopMode();
            CreateGameWindow(desktop, sf::Style::None, sf::State::Fullscreen);
        }
        else
        {
            // Return to windowed
            const sf::VideoMode mode(static_cast<sf::Vector2u>(EC.WindowSize));
            CreateGameWindow(mode, sf::Style::Default, sf::State::Windowed);

            // Restore position
            setPosition(WindowedPosition);
        }
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
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
        : WindowedPosition{ vec2i(sf::VideoMode::getDesktopMode().size.x / 2, sf::VideoMode::getDesktopMode().size.y / 2) }
        , bIsFullscreen{EC.FullscreenMode }
    {
        create(sf::VideoMode(vec2u(EC.WindowSize)), EC.WindowName);
        SetIcon();
    }

    void GameWindow::SetIcon()
    {
        auto IconTexture = Asset().LoadTexture(EC.WindowIcon);
        const auto& image = IconTexture->copyToImage();
        setIcon(image);
    }

    void GameWindow::HandleEvents()
    {
        while (const auto Event = pollEvent())
        {
            Event->visit(GameWindowEventHandler{*this});
        }
    }

    void GameWindow::EventToggleBorderlessFullscreen()
    {
        bIsFullscreen = !bIsFullscreen;

        if (!bIsFullscreen)
        {
            create(sf::VideoMode(vec2u(EC.WindowSize)), EC.WindowName, sf::Style::Default);
            setPosition(WindowedPosition);
        }
        else
        {
            WindowedPosition = getPosition();
            auto desktop = sf::VideoMode::getDesktopMode();
            create(desktop, EC.WindowName, sf::Style::None, sf::State::Fullscreen);
        }

        SetIcon();
        EventWindowResized();
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
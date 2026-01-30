// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/GameWindow.h"
#include "EngineConfig.h"
#include "Utility/Log.h"

#include "AssetDirectory/PakDirectory.h"

namespace we
{
    GameWindow::GameWindow()
        : WindowIcon{"Content/Assets/Icon/icon.png"}
        , WindowedPosition{ vec2i(sf::VideoMode::getDesktopMode().size.x / 2, sf::VideoMode::getDesktopMode().size.y / 2) }
        , bIsFullscreen{EC.FullscreenMode }
    {
        create(sf::VideoMode(vec2u(EC.WindowSize)), EC.WindowName);
        SetIcon();

        // --- DEBUG: PakDirectory test ---
        PakDirectory Pak("Contents.pak");

        LOG("Exists Config? {}", Pak.Exists("Config"));
        LOG("Exists Config/EngineConfig.json? {}", Pak.Exists("Config/EngineConfig.json"));
        LOG("Exists Save? {}", Pak.Exists("Save"));
        LOG("Exists Assets/Textures/Defualt? {}", Pak.Exists("Assets/Textures/Default"));
        LOG("Exists Content/Assets/Icon/icon.png? {}", Pak.Exists("Content/Assets/Icon/icon.png"));
    }

    void GameWindow::SetIcon()
    {
        setIcon(WindowIcon);
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
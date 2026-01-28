// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/GameWindow.h"
#include "EngineConfig.h"

namespace we
{
	GameWindow::GameWindow()
	{
		// Store default windowed size and position
        WindowedPosition = { vec2i(sf::VideoMode::getDesktopMode().size.x / 2, sf::VideoMode::getDesktopMode().size.y / 2) };
		create(sf::VideoMode(vec2u(EC.WindowSize)), EC.WindowName);
		setIcon(sf::Image("Content/Assets/Icon/icon.png"));
	}

	void GameWindow::ToggleBorderlessFullscreen()
	{
        bIsFullscreen = !bIsFullscreen;
        // Save current windowed size & position before switching
        if (!bIsFullscreen)
        {
            // going back to windowed
            create(sf::VideoMode(vec2u(EC.WindowSize)), EC.WindowName, sf::Style::Default);
            setPosition(WindowedPosition);
        }
        else
        {
            // going to borderless fullscreen
            WindowedPosition = getPosition();

            auto desktop = sf::VideoMode::getDesktopMode();
            create(desktop, EC.WindowName, sf::Style::None, sf::State::Fullscreen);
            setPosition({ 0, 0 });
        }

        // Optional: restore icon after recreate
        setIcon(sf::Image("Content/Assets/Icon/icon.png"));
	}
}
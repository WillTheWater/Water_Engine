// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/Application.h"
#include <SFML/Graphics.hpp>

namespace we
{
	Application::Application()
	{
		mGameWindow = make_unique<GameWindow>();
	}

	void Application::Run()
	{
		sf::CircleShape shape(100.f);
		shape.setPosition({ 1000,600 });
		shape.setFillColor(sf::Color::Green);

		// Main loop
		while (mGameWindow->isOpen())
		{
			while (const optional Event = mGameWindow->pollEvent())
			{
				if (Event->is<sf::Event::Closed>())
					mGameWindow->close();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				{
					mGameWindow->ToggleBorderlessFullscreen();
				}
			}

			mGameWindow->clear();
			mGameWindow->draw(shape);
			mGameWindow->display();
		}
	}
}
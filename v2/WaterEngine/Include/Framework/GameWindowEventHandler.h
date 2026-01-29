// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

#include <SFML/Window/Event.hpp>

namespace we
{
	struct GameWindowEventHandler
	{
		class GameWindow& Window;
	
		void operator()(const sf::Event::Closed&);
		void operator()(const sf::Event::Resized&);
		void operator()(const sf::Event::FocusLost&);
		void operator()(const sf::Event::FocusGained&);

		void operator()(const sf::Event::JoystickConnected&);
		void operator()(const sf::Event::JoystickDisconnected&);
		void operator()(const sf::Event::JoystickButtonPressed&);
		void operator()(const sf::Event::JoystickButtonReleased&);

		void operator()(const sf::Event::KeyPressed&);
		void operator()(const sf::Event::KeyReleased&);

		void operator()(const sf::Event::MouseButtonPressed&);
		void operator()(const sf::Event::MouseButtonReleased&);

		void operator()(const auto&) {}
	};
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include <SFML/Window/Event.hpp>

namespace we
{
	class InputSubsystem;

	struct InputEventHandler
	{
		InputSubsystem& Input;

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
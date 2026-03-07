// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <SFML/Window/Event.hpp>

namespace we
{
	class GUISubsystem;

	struct GUIEventHandler
	{
		GUISubsystem& GUI;
		bool Consumed = false;

		void operator()(const sf::Event::MouseButtonPressed&);
		void operator()(const sf::Event::MouseButtonReleased&);
		void operator()(const sf::Event::MouseMoved&);
		void operator()(const sf::Event::KeyPressed&);
		void operator()(const sf::Event::KeyReleased&);
		void operator()(const sf::Event::MouseWheelScrolled&);

		void operator()(const auto&) {}
	};
}

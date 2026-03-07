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

		void operator()(const event::MouseButtonPressed&);
		void operator()(const event::MouseButtonReleased&);
		void operator()(const event::MouseMoved&);
		void operator()(const event::KeyPressed&);
		void operator()(const event::KeyReleased&);
		void operator()(const event::MouseWheelScrolled&);

		void operator()(const auto&) {}
	};
}

// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <SFML/Window/Event.hpp>

namespace we
{
	class InputSubsystem;

	struct InputEventHandler
	{
		InputSubsystem& Input;

		void operator()(const event::JoystickConnected&);
		void operator()(const event::JoystickDisconnected&);
		void operator()(const event::JoystickButtonPressed&);
		void operator()(const event::JoystickButtonReleased&);

		void operator()(const event::KeyPressed&);
		void operator()(const event::KeyReleased&);

		void operator()(const event::MouseMoved&);
		void operator()(const event::MouseButtonPressed&);
		void operator()(const event::MouseButtonReleased&);

		void operator()(const auto&) {}
	};
}
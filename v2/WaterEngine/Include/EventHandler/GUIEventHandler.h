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

		vec2i TransformMousePos(int x, int y) const;

		void operator()(const event::MouseButtonPressed& e);
		void operator()(const event::MouseButtonReleased& e);
		void operator()(const event::MouseMoved& e);
		void operator()(const event::MouseWheelScrolled& e);
		void operator()(const event::KeyPressed& e);
		void operator()(const event::KeyReleased& e);
		void operator()(const auto&) {}
	};
}
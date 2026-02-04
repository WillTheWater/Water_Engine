// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
	class GameWindow;

	class GUISubsystem
	{
	public:
		GUISubsystem(GameWindow& Window);

	private:
		GameWindow& Window;

	private:
		friend class WaterEngine;
		bool HandleEvents(const sf::Event&);
		void Render();

	private:
		void HandleEvent(const sf::Event::JoystickButtonPressed&);
		void HandleEvent(const sf::Event::JoystickButtonReleased&);
		void HandleEvent(const auto&) {}
	};
}
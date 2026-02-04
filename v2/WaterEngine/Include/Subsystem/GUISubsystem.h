// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <tgui/TGUI.hpp>

namespace we
{
	class GameWindow;

	class GUISubsystem
	{
	public:
		GUISubsystem(GameWindow& Window);

		void AddWidget(const tgui::Widget::Ptr& Widget);
		void RemoveWidget(const tgui::Widget::Ptr& Widget);

	private:
		GameWindow& Window;
		tgui::Gui GUI;

	private:
		friend class WaterEngine;
		bool HandleEvents(const sf::Event&);
		void Render();

	private:
		void HandleEvent(const sf::Event::Resized&);
		void HandleEvent(const sf::Event::JoystickButtonPressed&);
		void HandleEvent(const sf::Event::JoystickButtonReleased&);
		void HandleEvent(const auto&) {}
	};
}
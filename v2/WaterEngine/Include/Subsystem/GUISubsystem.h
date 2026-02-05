// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"

namespace we
{
	class Widget;
	class GameWindow;

	class GUISubsystem
	{
	public:
		explicit GUISubsystem(GameWindow& Window);
		~GUISubsystem();

		void ProcessEvent(const sf::Event& Event);
		void Update(float DeltaTime);
		void Render();

		void AddWidget(shared<Widget> InWidget);
		void RemoveWidget(Widget* InWidget);
		void Clear();

		bool IsMousePressed() const { return bMousePressed; }

	private:
		GameWindow& Window;
		list<shared<Widget>> Widgets;
		bool bMousePressed = false;

	private:
		void HandleEvent(const sf::Event::MouseButtonPressed&);
		void HandleEvent(const sf::Event::MouseButtonReleased&);
		void HandleEvent(const sf::Event::JoystickButtonPressed&);
		void HandleEvent(const sf::Event::JoystickButtonReleased&);
		void HandleEvent(const auto&) {}
	};
}
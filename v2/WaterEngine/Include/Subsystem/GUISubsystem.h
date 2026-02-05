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
	class Cursor;

	class GUISubsystem
	{
	public:
		explicit GUISubsystem(GameWindow& InWindow);
		~GUISubsystem();

		void Update(float DeltaTime);
		void Render();

		void HandleMousePress();
		void HandleMouseRelease();

		void AddWidget(shared<Widget> InWidget);
		void RemoveWidget(Widget* InWidget);
		void Clear();

		vec2f GetMousePosition() const;

	private:
		GameWindow& Window;
		list<shared<Widget>> Widgets;
		Widget* HoveredWidget = nullptr;
		Widget* PressedWidget = nullptr;
	};
}
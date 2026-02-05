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

		void Update(float DeltaTime);
		void Render();

		void AddWidget(shared<Widget> InWidget);
		void RemoveWidget(Widget* InWidget);
		void Clear();

	private:
		GameWindow& Window;
		list<shared<Widget>> Widgets;
	};
}
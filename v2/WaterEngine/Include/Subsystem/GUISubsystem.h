// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace we
{
	class GUISubsystem
	{
	public:
		GUISubsystem();

		static GUISubsystem& Get();

		void Initialize(renderTarget& ScreenUITarget, renderTarget& WorldUITarget);

		tgui::Gui& GetScreenUI() { return ScreenUI; }
		tgui::Gui& GetWorldUI() { return WorldUI; }

		// Returns true if event was consumed by GUI (don't pass to game)
		bool HandleEvent(const sf::Event& event);

		// Returns true if the GUI should block game input (e.g., mouse over widget)
		// Used to prevent game actions when interacting with UI
		bool ShouldBlockInput() const;

	private:
		static GUISubsystem* Instance;

		tgui::Gui ScreenUI;
		tgui::Gui WorldUI;
	};

	inline GUISubsystem& MakeGUI() { return GUISubsystem::Get(); }
}

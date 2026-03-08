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

		void SetCameraView(const view NewView);
		bool HandleEvent(const sf::Event& event);

	private:
		static GUISubsystem* Instance;

		tgui::Gui ScreenUI;
		tgui::Gui WorldUI;
	};

	inline GUISubsystem& MakeGUI() { return GUISubsystem::Get(); }
}
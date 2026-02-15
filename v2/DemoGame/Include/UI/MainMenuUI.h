// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h
#include "Utility/Delegate.h"

namespace we
{
	class Button;
	class Panel;
	struct EngineSubsystem;

	class MainMenuUI
	{
	public:
		explicit MainMenuUI(EngineSubsystem& Subsystem);
		~MainMenuUI();

	public:
		Delegate<>OnPlayButtonClicked;
		Delegate<>OnSettingsButtonClicked;

	private:
		void CreateUI();
		void OnPlayClicked();
		void OnSettingsClicked();
		void OnQuitClicked();

		EngineSubsystem& Subsystem;
		shared<Panel> MenuPanel;
		shared<Button> PlayButton;
		shared<Button> SettingsButton;
		shared<Button> QuitButton;
	};
}
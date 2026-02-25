// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"
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

		Delegate<> OnPlayClicked;
		Delegate<> OnSettingsClicked;
		Delegate<> OnQuitClicked;

		void Hide();
		void Show();
		bool IsVisible() const;

	private:
		void CreateUI();
		void OnPlayButtonClicked();
		void OnSettingsButtonClicked();
		void OnQuitButtonClicked();

		EngineSubsystem& Subsystem;
		shared<Panel> MenuPanel;
		shared<Button> PlayButton;
		shared<Button> SettingsButton;
		shared<Button> QuitButton;
	};
}

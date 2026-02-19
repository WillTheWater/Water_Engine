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

	class PauseUI
	{
	public:
		explicit PauseUI(EngineSubsystem& InSubsystem);
		~PauseUI();

		Delegate<> OnContinueClicked;
		Delegate<> OnSettingsClicked;
		Delegate<> OnQuitClicked;

		void Show();
		void Hide();
		bool IsVisible() const;
		void Toggle();

	private:
		void CreateUI();
		void OnContinueButtonClicked();
		void OnSettingsButtonClicked();
		void OnQuitButtonClicked();

	private:
		EngineSubsystem& Subsystem;
		shared<Panel> MenuPanel;
		shared<Button> ContinueButton;
		shared<Button> SettingsButton;
		shared<Button> QuitButton;
	};
}

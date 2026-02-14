// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"
#include "Utility/Delegate.h"

namespace we
{
	class Button;
	class Panel;
	struct EngineSubsystem;

	class SettingsMenuUI
	{
	public:
		SettingsMenuUI(EngineSubsystem& Subsystem);

		void Show();
		void Hide();

		bool IsVisible() const { return bVisible; }

	public:
		Delegate<>OnExitClicked;
		Delegate<>OnToggleFSClicked;

	private:
		void CreateUI();
		void OnExitButtonClicked();
		void ToggleFullscreen();

	private:
		EngineSubsystem& Subsystem;

		bool bVisible = false;

		shared<Panel> SettingsPanel;
		shared<Button> ToggleFullscreenButton;
		shared<Button> ExitButton;
	};
}
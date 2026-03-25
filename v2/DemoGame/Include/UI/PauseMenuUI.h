// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <TGUI/Widgets/Button.hpp>
#include "Utility/Delegate.h"

namespace we
{
	class PauseMenuUI
	{
	public:
		PauseMenuUI();
		
		void Initialize();
		void ClearWidgets();
		void Show();
		void Hide();
		void Toggle();
		bool IsVisible() const { return bVisible; }

		Delegate<> OnResumeClicked;
		Delegate<> OnSettingsClicked;
		Delegate<> OnMainMenuClicked;
		Delegate<> OnSaveAndQuitClicked;
		
	private:
		void SetupLayout();
		tgui::Button::Ptr CreateButton(const std::string& Text);
		void OnResume();
		void OnSettings();
		void OnMainMenu();
		void OnSaveAndQuit();
		
		bool bVisible = false;
		bool bInitialized = false;
	};
}

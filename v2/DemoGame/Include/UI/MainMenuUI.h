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
	class MainMenuUI
	{
	public:
		MainMenuUI();
		
		void Initialize();
		void ClearWidgets();
		void Show();
		void Hide();

		Delegate<> OnPlayButtonClicked;
		Delegate<> OnSettingsButtonClicked;
		Delegate<> OnQuitButtonClicked;
		
	private:
		void SetupLayout();
		tgui::Button::Ptr CreateButton(const std::string& Text, 
			tgui::Color HoverColor, tgui::Color DownColor);
		void OnPlayClicked();
		void OnSettingsClicked();
		void OnQuitClicked();
		
		bool bVisible = false;
		bool bInitialized = false;
	};
}
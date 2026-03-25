// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Slider.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include "Utility/Delegate.h"

namespace we
{
	class SettingsUI
	{
	public:
		SettingsUI();

		void Initialize();
		void ClearWidgets();
		void Show();
		void Hide();

		Delegate<> OnBackClicked;

	private:
		void SetupLayout();
		tgui::Button::Ptr CreateButton(const std::string& Text);
		tgui::Slider::Ptr CreateSlider();
		tgui::CheckBox::Ptr CreateCheckbox(const std::string& Text);
		void OnBackPressed();

	private:
		bool bVisible = false;
		bool bInitialized = false;
	};
}

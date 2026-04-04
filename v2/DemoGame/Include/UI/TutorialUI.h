// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Utility/Delegate.h"
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Button.hpp>

namespace we
{
	class TutorialUI
	{
	public:
		TutorialUI() = default;
		~TutorialUI();

		void Initialize();
		void ClearWidgets();
		void Show();
		void Hide();
		bool IsVisible() const { return bVisible; }

		Delegate<> OnContinueClicked;

	private:
		void SetupLayout();
		tgui::Button::Ptr CreateButton(const std::string& Text);
		void OnContinuePressed();

	private:
		bool bVisible = false;
		bool bInitialized = false;
		tgui::Panel::Ptr Background;
		tgui::Button::Ptr ContinueButton;
	};
}

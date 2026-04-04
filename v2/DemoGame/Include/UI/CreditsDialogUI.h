// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/UIController.h"
#include <TGUI/TGUI.hpp>

namespace we
{
	class CreditsDialogUI
	{
	public:
		CreditsDialogUI();
		~CreditsDialogUI();

		void Initialize();
		void Show();
		void Hide();
		void Tick(float DeltaTime);
		bool IsVisible() const;

		// Set dialog content
		void SetDialog(const vector<string>& Lines);
		
		// Advance to next line, returns false if dialog ended
		bool Advance();
		
		// Check if dialog is complete
		bool IsComplete() const;

		// Button callbacks
		void SetOnMainMenu(std::function<void()> Callback);
		void SetOnQuit(std::function<void()> Callback);

	private:
		void DisplayCurrentLine();
		void ShowEndButtons();
		void HideEndButtons();

	private:
		tgui::Panel::Ptr Background;
		tgui::Label::Ptr TextLabel;
		tgui::Label::Ptr ContinueArrow;
		
		// End buttons container
		tgui::HorizontalLayout::Ptr ButtonsLayout;
		tgui::Button::Ptr MainMenuButton;
		tgui::Button::Ptr QuitButton;
		
		vector<string> DialogLines;
		size_t CurrentLine = 0;
		bool bComplete = false;
		bool bInitialized = false;
		
		std::function<void()> OnMainMenu;
		std::function<void()> OnQuit;
		
		UIController Controller;
		
		static constexpr float Width = 800.0f;
		static constexpr float Height = 280.0f;
	};
}

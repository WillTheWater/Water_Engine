// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Slider.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include "UI/UIController.h"
#include "Utility/Delegate.h"

namespace we
{
	class SaveSubsystem;

	class SettingsUI
	{
	public:
		SettingsUI();

		void Initialize(SaveSubsystem& InSave);
		void ClearWidgets();
		void Show();
		void Hide();
		void Tick(float DeltaTime);

		Delegate<> OnBackClicked;

	private:
		void SetupLayout();
		tgui::Button::Ptr CreateButton(const std::string& Text);
		tgui::Slider::Ptr CreateSlider();
		tgui::CheckBox::Ptr CreateCheckbox(const std::string& Text);
		void OnBackPressed();

		// Settings callbacks
		void LoadSettings();
		void SaveSettings();
		
		void OnMuteChanged();
		void OnMasterVolumeChanged();
		void OnMusicVolumeChanged();
		void OnAmbientVolumeChanged();
		void OnSFXVolumeChanged();
		void OnVoiceVolumeChanged();
		void OnUIVolumeChanged();
		void OnFullscreenChanged();

	private:
		bool bVisible = false;
		bool bInitialized = false;
		SaveSubsystem* Save = nullptr;
		UIController Controller;

		// Widgets
		tgui::CheckBox::Ptr MuteCheckbox;
		tgui::CheckBox::Ptr FullscreenCheckbox;
		tgui::Slider::Ptr MasterSlider;
		tgui::Slider::Ptr MusicSlider;
		tgui::Slider::Ptr AmbientSlider;
		tgui::Slider::Ptr SFXSlider;
		tgui::Slider::Ptr VoiceSlider;
		tgui::Slider::Ptr UISlider;
	};
}

// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/SettingsUI.h"
#include "UI/UIStyle.h"
#include "Subsystem/GuiSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/SaveSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Config/GameConfig.h"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Slider.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Panel.hpp>

namespace we
{
	SettingsUI::SettingsUI() = default;

	void SettingsUI::Initialize(SaveSubsystem& InSave)
	{
		if (bInitialized)
			return;

		Save = &InSave;
		SetupLayout();
		LoadSettings();
		bInitialized = true;
		Hide();
	}

	void SettingsUI::ClearWidgets()
	{
		if (!bInitialized)
			return;

		auto& GUI = MakeGUI().GetScreenUI();
		GUI.remove(GUI.get("SettingsOverlay"));

		bInitialized = false;
		bVisible = false;
	}

	void SettingsUI::SetupLayout()
	{
		auto& GUI = MakeGUI().GetScreenUI();

		// Background overlay (semi-transparent dark)
		auto Overlay = tgui::Panel::create({ "100%", "100%" });
		Overlay->getRenderer()->setBackgroundColor(UIStyle::GetColors().PanelBackground);
		Overlay->setPosition(0, 0);

		// Main content panel - no scrolling
		auto ContentPanel = UIStyle::CreatePanel({ "30%", "60%" });
		ContentPanel->setOrigin(0.5f, 0.5f);
		ContentPanel->setPosition("50%", "50%");

		float CurrentY = 25; // Starting Y position
		float RowHeight = 45;  // Increased from 35 to prevent overlapping
		float SectionSpacing = 20;  // Increased spacing between sections
		float LabelSpacing = 8;

		// === AUDIO SECTION ===
		auto AudioLabel = UIStyle::CreateLabel("AUDIO", UILabelStyle::Section);
		AudioLabel->setPosition("5%", CurrentY);
		ContentPanel->add(AudioLabel);
		CurrentY += RowHeight;

		// Mute toggle
		auto MuteLabel = UIStyle::CreateLabel("Mute All", UILabelStyle::Body);
		MuteLabel->setPosition("5%", CurrentY);
		ContentPanel->add(MuteLabel);
		MuteCheckbox = UIStyle::CreateCheckbox("");
		MuteCheckbox->setPosition("85%", CurrentY);
		MuteCheckbox->onChange([this]() { OnMuteChanged(); });
		ContentPanel->add(MuteCheckbox);
		CurrentY += RowHeight + LabelSpacing;

		// Channel sliders helper
		auto AddChannelSlider = [&](const string& Name, tgui::Slider::Ptr& Slider, auto Callback)
		{
			auto Label = UIStyle::CreateLabel(Name, UILabelStyle::Body);
			Label->setPosition("5%", CurrentY);
			ContentPanel->add(Label);
			
			Slider = UIStyle::CreateSlider();
			Slider->setSize({ "50%", "20" });
			Slider->setPosition("45%", CurrentY + 5);
			Slider->onValueChange(Callback);
			ContentPanel->add(Slider);
			
			CurrentY += RowHeight;
		};

		AddChannelSlider("Master", MasterSlider, [this]() { OnMasterVolumeChanged(); });
		AddChannelSlider("Music", MusicSlider, [this]() { OnMusicVolumeChanged(); });
		AddChannelSlider("Ambient", AmbientSlider, [this]() { OnAmbientVolumeChanged(); });
		AddChannelSlider("SFX", SFXSlider, [this]() { OnSFXVolumeChanged(); });
		AddChannelSlider("Voice", VoiceSlider, [this]() { OnVoiceVolumeChanged(); });
		AddChannelSlider("UI", UISlider, [this]() { OnUIVolumeChanged(); });

		CurrentY += SectionSpacing;

		// === VIDEO SECTION ===
		auto VideoLabel = UIStyle::CreateLabel("VIDEO", UILabelStyle::Section);
		VideoLabel->setPosition("5%", CurrentY);
		ContentPanel->add(VideoLabel);
		CurrentY += RowHeight;

		// Fullscreen toggle
		auto FullscreenLabel = UIStyle::CreateLabel("Fullscreen", UILabelStyle::Body);
		FullscreenLabel->setPosition("5%", CurrentY);
		ContentPanel->add(FullscreenLabel);
		FullscreenCheckbox = UIStyle::CreateCheckbox("");
		FullscreenCheckbox->setPosition("85%", CurrentY);
		FullscreenCheckbox->onChange([this]() { OnFullscreenChanged(); });
		ContentPanel->add(FullscreenCheckbox);
		CurrentY += RowHeight + SectionSpacing;

		// === BACK BUTTON ===
		auto BackButton = UIStyle::CreateButton("BACK");
		BackButton->setSize({ "40%", "40" });
		BackButton->setPosition("30%", "85%");
		BackButton->onPress([this]() { OnBackPressed(); });
		ContentPanel->add(BackButton);

		Overlay->add(ContentPanel);
		GUI.add(Overlay, "SettingsOverlay");
	}

	void SettingsUI::LoadSettings()
	{
		if (!Save)
			return;

		float DefaultVolume = 0.5f;

		// Master
		float MasterVol = Save->Get<float>(SAVE_MASTER_VOLUME, DefaultVolume);
		MasterSlider->setValue(static_cast<int>(MasterVol * 100.0f));
		PlayAudio().SetMasterVolume(MasterVol);

		bool MasterMuted = Save->Get<bool>(SAVE_MASTER_MUTED, false);
		MuteCheckbox->setChecked(MasterMuted);
		PlayAudio().SetMasterMuted(MasterMuted);

		// Music
		float MusicVol = Save->Get<float>(SAVE_MUSIC_VOLUME, DefaultVolume);
		MusicSlider->setValue(static_cast<int>(MusicVol * 100.0f));
		PlayAudio().SetChannelVolume(AudioChannel::Music, MusicVol);

		bool MusicMuted = Save->Get<bool>(SAVE_MUSIC_MUTED, false);
		PlayAudio().SetChannelMuted(AudioChannel::Music, MusicMuted);

		// Ambient
		float AmbientVol = Save->Get<float>(SAVE_AMBIENT_VOLUME, DefaultVolume);
		AmbientSlider->setValue(static_cast<int>(AmbientVol * 100.0f));
		PlayAudio().SetChannelVolume(AudioChannel::Ambient, AmbientVol);

		bool AmbientMuted = Save->Get<bool>(SAVE_AMBIENT_MUTED, false);
		PlayAudio().SetChannelMuted(AudioChannel::Ambient, AmbientMuted);

		// SFX
		float SFXVol = Save->Get<float>(SAVE_SFX_VOLUME, DefaultVolume);
		SFXSlider->setValue(static_cast<int>(SFXVol * 100.0f));
		PlayAudio().SetChannelVolume(AudioChannel::SFX, SFXVol);

		bool SFXMuted = Save->Get<bool>(SAVE_SFX_MUTED, false);
		PlayAudio().SetChannelMuted(AudioChannel::SFX, SFXMuted);

		// Voice
		float VoiceVol = Save->Get<float>(SAVE_VOICE_VOLUME, DefaultVolume);
		VoiceSlider->setValue(static_cast<int>(VoiceVol * 100.0f));
		PlayAudio().SetChannelVolume(AudioChannel::Voice, VoiceVol);

		bool VoiceMuted = Save->Get<bool>(SAVE_VOICE_MUTED, false);
		PlayAudio().SetChannelMuted(AudioChannel::Voice, VoiceMuted);

		// UI
		float UIVol = Save->Get<float>(SAVE_UI_VOLUME, DefaultVolume);
		UISlider->setValue(static_cast<int>(UIVol * 100.0f));
		PlayAudio().SetChannelVolume(AudioChannel::UI, UIVol);

		bool UIMuted = Save->Get<bool>(SAVE_UI_MUTED, false);
		PlayAudio().SetChannelMuted(AudioChannel::UI, UIMuted);

		// Fullscreen (saved state only, no logic yet)
		bool bFullscreen = Save->Get<bool>(SAVE_FULLSCREEN, false);
		FullscreenCheckbox->setChecked(bFullscreen);
	}

	void SettingsUI::SaveSettings()
	{
		if (Save)
			Save->Save();
	}

	void SettingsUI::OnMuteChanged()
	{
		bool bMuted = MuteCheckbox->isChecked();
		PlayAudio().SetMasterMuted(bMuted);
		if (Save)
			Save->Set(SAVE_MASTER_MUTED, bMuted);
	}

	void SettingsUI::OnMasterVolumeChanged()
	{
		float Volume = MasterSlider->getValue() / 100.0f;
		PlayAudio().SetMasterVolume(Volume);
		if (Save)
			Save->Set(SAVE_MASTER_VOLUME, Volume);
	}

	void SettingsUI::OnMusicVolumeChanged()
	{
		float Volume = MusicSlider->getValue() / 100.0f;
		PlayAudio().SetChannelVolume(AudioChannel::Music, Volume);
		if (Save)
			Save->Set(SAVE_MUSIC_VOLUME, Volume);
	}

	void SettingsUI::OnAmbientVolumeChanged()
	{
		float Volume = AmbientSlider->getValue() / 100.0f;
		PlayAudio().SetChannelVolume(AudioChannel::Ambient, Volume);
		if (Save)
			Save->Set(SAVE_AMBIENT_VOLUME, Volume);
	}

	void SettingsUI::OnSFXVolumeChanged()
	{
		float Volume = SFXSlider->getValue() / 100.0f;
		PlayAudio().SetChannelVolume(AudioChannel::SFX, Volume);
		if (Save)
			Save->Set(SAVE_SFX_VOLUME, Volume);
	}

	void SettingsUI::OnVoiceVolumeChanged()
	{
		float Volume = VoiceSlider->getValue() / 100.0f;
		PlayAudio().SetChannelVolume(AudioChannel::Voice, Volume);
		if (Save)
			Save->Set(SAVE_VOICE_VOLUME, Volume);
	}

	void SettingsUI::OnUIVolumeChanged()
	{
		float Volume = UISlider->getValue() / 100.0f;
		PlayAudio().SetChannelVolume(AudioChannel::UI, Volume);
		if (Save)
			Save->Set(SAVE_UI_VOLUME, Volume);
	}

	void SettingsUI::OnFullscreenChanged()
	{
		bool bFullscreen = FullscreenCheckbox->isChecked();
		if (Save)
			Save->Set(SAVE_FULLSCREEN, bFullscreen);
		MakeGUI().RequestFullscreen(bFullscreen);
	}

	void SettingsUI::OnBackPressed()
	{
		SaveSettings();
		OnBackClicked.Broadcast();
	}

	void SettingsUI::Show()
	{
		if (!bInitialized)
			return;

		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Overlay = GUI.get("SettingsOverlay"))
			Overlay->setVisible(true);

		bVisible = true;
		Controller.Enable();
	}

	void SettingsUI::Hide()
	{
		if (!bInitialized)
			return;

		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Overlay = GUI.get("SettingsOverlay"))
			Overlay->setVisible(false);

		bVisible = false;
		Controller.Disable();
	}

	void SettingsUI::Tick(float DeltaTime)
	{
		if (!bVisible)
			return;

		Controller.Tick(DeltaTime);
	}
}

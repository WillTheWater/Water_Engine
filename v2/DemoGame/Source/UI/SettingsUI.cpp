// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/SettingsUI.h"
#include "Subsystem/GuiSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/SaveSubsystem.h"
#include "Config/GameConfig.h"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Slider.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <TGUI/Widgets/HorizontalLayout.hpp>

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
		Overlay->getRenderer()->setBackgroundColor(tgui::Color{ 0, 0, 0, 180 });
		Overlay->setPosition(0, 0);

		// Main content layout
		auto Layout = tgui::VerticalLayout::create();
		Layout->setSize({ "30%", "60%" });
		Layout->setOrigin(0.5f, 0.5f);
		Layout->setPosition("50%", "50%");
		Layout->getRenderer()->setSpaceBetweenWidgets(10);
		Layout->getRenderer()->setPadding(20);

		// === AUDIO SECTION ===
		auto AudioLabel = tgui::Label::create("AUDIO");
		AudioLabel->setTextSize(24);
		AudioLabel->getRenderer()->setTextColor(tgui::Color::White);
		Layout->add(AudioLabel);

		// Mute toggle (manual positioning to avoid stretching)
		auto MuteRow = tgui::Panel::create({ "100%", "30" });
		MuteRow->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		auto MuteLabel = tgui::Label::create("Mute All");
		MuteLabel->setTextSize(18);
		MuteLabel->setPosition(0, 5);
		MuteLabel->getRenderer()->setTextColor(tgui::Color::White);
		MuteRow->add(MuteLabel);
		MuteCheckbox = CreateCheckbox("");
		MuteCheckbox->setPosition("85%", 2);
		MuteCheckbox->setSize(25, 25);
		MuteCheckbox->onChange([this]() { OnMuteChanged(); });
		MuteRow->add(MuteCheckbox);
		Layout->add(MuteRow);

		// Channel sliders
		auto AddChannelSlider = [&](const std::string& Name, tgui::Slider::Ptr& Slider, auto Callback)
		{
			auto Row = tgui::HorizontalLayout::create();
			Row->setSize({ "100%", "30" });
			auto Label = tgui::Label::create(Name);
			Label->setTextSize(16);
			Label->getRenderer()->setTextColor(tgui::Color::White);
			Label->setSize({ "80", "100%" });
			Row->add(Label);
			Slider = CreateSlider();
			Slider->onValueChange(Callback);
			Row->add(Slider);
			Layout->add(Row);
		};

		AddChannelSlider("Master", MasterSlider, [this]() { OnMasterVolumeChanged(); });
		AddChannelSlider("Music", MusicSlider, [this]() { OnMusicVolumeChanged(); });
		AddChannelSlider("Ambient", AmbientSlider, [this]() { OnAmbientVolumeChanged(); });
		AddChannelSlider("SFX", SFXSlider, [this]() { OnSFXVolumeChanged(); });
		AddChannelSlider("Voice", VoiceSlider, [this]() { OnVoiceVolumeChanged(); });
		AddChannelSlider("UI", UISlider, [this]() { OnUIVolumeChanged(); });

		// Spacer
		Layout->add(tgui::Label::create(""), "Spacer1");

		// === VIDEO SECTION ===
		auto VideoLabel = tgui::Label::create("VIDEO");
		VideoLabel->setTextSize(24);
		VideoLabel->getRenderer()->setTextColor(tgui::Color::White);
		Layout->add(VideoLabel);

		// Fullscreen toggle (manual positioning to avoid stretching)
		auto FullscreenRow = tgui::Panel::create({ "100%", "30" });
		FullscreenRow->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		auto FullscreenLabel = tgui::Label::create("Fullscreen");
		FullscreenLabel->setTextSize(18);
		FullscreenLabel->setPosition(0, 5);
		FullscreenLabel->getRenderer()->setTextColor(tgui::Color::White);
		FullscreenRow->add(FullscreenLabel);
		FullscreenCheckbox = CreateCheckbox("");
		FullscreenCheckbox->setPosition("85%", 2);
		FullscreenCheckbox->setSize(25, 25);
		FullscreenCheckbox->onChange([this]() { OnFullscreenChanged(); });
		FullscreenRow->add(FullscreenCheckbox);
		Layout->add(FullscreenRow);

		// Spacer (pushes back button to bottom)
		Layout->add(tgui::Label::create(""), "Spacer2");

		// === BACK BUTTON ===
		auto BackButton = CreateButton("BACK");
		BackButton->onPress([this]() { OnBackPressed(); });
		Layout->add(BackButton, "BackButton");

		Overlay->add(Layout);
		GUI.add(Overlay, "SettingsOverlay");
	}

	tgui::Button::Ptr SettingsUI::CreateButton(const std::string& Text)
	{
		auto Button = tgui::Button::create(Text);
		Button->setTextSize(24);
		Button->setFocusable(false);

		auto Renderer = Button->getRenderer();
		Renderer->setBackgroundColor(tgui::Color{ 0, 16, 31 });
		Renderer->setBackgroundColorHover(tgui::Color{ 47, 121, 142 });
		Renderer->setBackgroundColorDown(tgui::Color{ 133, 120, 81 });
		Renderer->setTextColor(tgui::Color::White);
		Renderer->setTextColorHover(tgui::Color::White);
		Renderer->setTextColorDown(tgui::Color::White);
		Renderer->setBorderColor(tgui::Color::White);
		Renderer->setBorders(tgui::Outline(2));

		return Button;
	}

	tgui::Slider::Ptr SettingsUI::CreateSlider()
	{
		auto Slider = tgui::Slider::create();
		Slider->setMinimum(0);
		Slider->setMaximum(100);
		Slider->setValue(100);
		Slider->setStep(1);

		auto Renderer = Slider->getRenderer();
		Renderer->setTrackColor(tgui::Color{ 0, 16, 31 });
		Renderer->setTrackColorHover(tgui::Color{ 47, 121, 142 });
		Renderer->setThumbColor(tgui::Color{ 133, 120, 81 });
		Renderer->setThumbColorHover(tgui::Color{ 200, 180, 120 });
		Renderer->setBorderColor(tgui::Color::White);
		Renderer->setBorders(tgui::Outline(2));

		return Slider;
	}

	tgui::CheckBox::Ptr SettingsUI::CreateCheckbox(const std::string& Text)
	{
		auto Checkbox = tgui::CheckBox::create(Text);
		Checkbox->setTextSize(18);

		auto Renderer = Checkbox->getRenderer();
		Renderer->setBackgroundColor(tgui::Color{ 0, 16, 31 });
		Renderer->setBackgroundColorHover(tgui::Color{ 47, 121, 142 });
		Renderer->setBackgroundColorChecked(tgui::Color{ 133, 120, 81 });
		Renderer->setBackgroundColorCheckedHover(tgui::Color{ 200, 180, 120 });
		Renderer->setBorderColor(tgui::Color::White);
		Renderer->setBorders(tgui::Outline(2));
		Renderer->setTextColor(tgui::Color::White);

		return Checkbox;
	}

	void SettingsUI::LoadSettings()
	{
		if (!Save)
			return;

		// Master
		float MasterVol = Save->Get<float>(SAVE_MASTER_VOLUME, 1.0f);
		MasterSlider->setValue(static_cast<int>(MasterVol * 100.0f));
		PlayAudio().SetMasterVolume(MasterVol);

		bool MasterMuted = Save->Get<bool>(SAVE_MASTER_MUTED, false);
		MuteCheckbox->setChecked(MasterMuted);
		PlayAudio().SetMasterMuted(MasterMuted);

		// Music
		float MusicVol = Save->Get<float>(SAVE_MUSIC_VOLUME, 1.0f);
		MusicSlider->setValue(static_cast<int>(MusicVol * 100.0f));
		PlayAudio().SetChannelVolume(AudioChannel::Music, MusicVol);

		bool MusicMuted = Save->Get<bool>(SAVE_MUSIC_MUTED, false);
		PlayAudio().SetChannelMuted(AudioChannel::Music, MusicMuted);

		// Ambient
		float AmbientVol = Save->Get<float>(SAVE_AMBIENT_VOLUME, 1.0f);
		AmbientSlider->setValue(static_cast<int>(AmbientVol * 100.0f));
		PlayAudio().SetChannelVolume(AudioChannel::Ambient, AmbientVol);

		bool AmbientMuted = Save->Get<bool>(SAVE_AMBIENT_MUTED, false);
		PlayAudio().SetChannelMuted(AudioChannel::Ambient, AmbientMuted);

		// SFX
		float SFXVol = Save->Get<float>(SAVE_SFX_VOLUME, 1.0f);
		SFXSlider->setValue(static_cast<int>(SFXVol * 100.0f));
		PlayAudio().SetChannelVolume(AudioChannel::SFX, SFXVol);

		bool SFXMuted = Save->Get<bool>(SAVE_SFX_MUTED, false);
		PlayAudio().SetChannelMuted(AudioChannel::SFX, SFXMuted);

		// Voice
		float VoiceVol = Save->Get<float>(SAVE_VOICE_VOLUME, 1.0f);
		VoiceSlider->setValue(static_cast<int>(VoiceVol * 100.0f));
		PlayAudio().SetChannelVolume(AudioChannel::Voice, VoiceVol);

		bool VoiceMuted = Save->Get<bool>(SAVE_VOICE_MUTED, false);
		PlayAudio().SetChannelMuted(AudioChannel::Voice, VoiceMuted);

		// UI
		float UIVol = Save->Get<float>(SAVE_UI_VOLUME, 1.0f);
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
	}

	void SettingsUI::Hide()
	{
		if (!bInitialized)
			return;

		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Overlay = GUI.get("SettingsOverlay"))
			Overlay->setVisible(false);

		bVisible = false;
	}
}

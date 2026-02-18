// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#include "UI/SettingsController.h"
#include "UI/SettingsMenuUI.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/SaveLoadSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	// Save keys
	constexpr stringView KEY_MASTER_VOLUME = "Settings.MasterVolume";
	constexpr stringView KEY_MUSIC_VOLUME = "Settings.MusicVolume";
	constexpr stringView KEY_AMBIENT_VOLUME = "Settings.AmbientVolume";
	constexpr stringView KEY_SFX_VOLUME = "Settings.SFXVolume";
	constexpr stringView KEY_FULLSCREEN = "Settings.Fullscreen";
	constexpr stringView KEY_VSYNC = "Settings.VSync";
	constexpr stringView KEY_SMOOTH_RENDER = "Settings.SmoothRender";
	constexpr stringView KEY_DEADZONE = "Settings.Deadzone";
	constexpr stringView KEY_CURSOR_SPEED = "Settings.CursorSpeed";

	// AudioSubsystem uses 0-100 range, sliders use 0-1 range
	constexpr float SLIDER_TO_VOLUME(float Slider) { return Slider * 100.0f; }
	constexpr float VOLUME_TO_SLIDER(float Volume) { return Volume / 100.0f; }

	SettingsController::SettingsController(EngineSubsystem& InSubsystem, SettingsMenuUI& InUI)
		: WidgetController(InSubsystem)
		, UI(InUI)
	{
	}

	void SettingsController::BindUI()
	{
		// Audio volume sliders - save to SaveLoad AND apply to AudioSubsystem immediately
		if (auto Slider = UI.GetMasterSlider())
		{
			Slider->OnValueChanged.Bind([this](float Value) { OnMasterVolumeChanged(Value); });
		}
		if (auto Slider = UI.GetMusicSlider())
		{
			Slider->OnValueChanged.Bind([this](float Value) { OnMusicVolumeChanged(Value); });
		}
		if (auto Slider = UI.GetAmbientSlider())
		{
			Slider->OnValueChanged.Bind([this](float Value) { OnAmbientVolumeChanged(Value); });
		}
		if (auto Slider = UI.GetSFXSlider())
		{
			Slider->OnValueChanged.Bind([this](float Value) { OnSFXVolumeChanged(Value); });
		}

		// Video toggles
		if (auto Check = UI.GetFullscreenCheck())
		{
			Check->OnToggled.Bind([this](bool bChecked) { OnFullscreenToggled(bChecked); });
		}
		if (auto Check = UI.GetVSyncCheck())
		{
			Check->OnToggled.Bind([this](bool bChecked) { OnVSyncToggled(bChecked); });
		}
		if (auto Check = UI.GetSmoothRenderCheck())
		{
			Check->OnToggled.Bind([this](bool bChecked) { OnSmoothRenderToggled(bChecked); });
		}

		// Gameplay sliders
		if (auto Slider = UI.GetDeadzoneSlider())
		{
			Slider->OnValueChanged.Bind([this](float Value) { OnDeadzoneChanged(Value); });
		}
		if (auto Slider = UI.GetCursorSpeedSlider())
		{
			Slider->OnValueChanged.Bind([this](float Value) { OnCursorSpeedChanged(Value); });
		}

		LOG("SettingsController: UI bindings created");
	}

	void SettingsController::SyncUIFromSettings()
	{
		auto& Save = *Subsystem.SaveLoad;
		auto& Audio = *Subsystem.Audio;
		
		// Audio: Use current AudioSubsystem values (already set by DemoGameInstance on startup)
		// Slider shows 0-1, AudioSubsystem stores 0-100
		if (auto Slider = UI.GetMasterSlider())
		{
			float Volume = Audio.GetGlobalVolume(); // 0-100
			float SliderValue = VOLUME_TO_SLIDER(Volume); // 0-1
			Slider->SetValue(SliderValue);
			if (auto Text = UI.GetMasterValue())
				Text->SetText(ValueToPercentText(SliderValue));
		}
		if (auto Slider = UI.GetMusicSlider())
		{
			float Volume = Audio.GetChannelVolume(AudioChannel::Music);
			float SliderValue = VOLUME_TO_SLIDER(Volume);
			Slider->SetValue(SliderValue);
			if (auto Text = UI.GetMusicValue())
				Text->SetText(ValueToPercentText(SliderValue));
		}
		if (auto Slider = UI.GetAmbientSlider())
		{
			float Volume = Audio.GetChannelVolume(AudioChannel::Ambient);
			float SliderValue = VOLUME_TO_SLIDER(Volume);
			Slider->SetValue(SliderValue);
			if (auto Text = UI.GetAmbientValue())
				Text->SetText(ValueToPercentText(SliderValue));
		}
		if (auto Slider = UI.GetSFXSlider())
		{
			float Volume = Audio.GetChannelVolume(AudioChannel::SFX);
			float SliderValue = VOLUME_TO_SLIDER(Volume);
			Slider->SetValue(SliderValue);
			if (auto Text = UI.GetSFXValue())
				Text->SetText(ValueToPercentText(SliderValue));
		}

		// Video settings (from SaveLoad only - not yet hooked to subsystems)
		if (auto Check = UI.GetFullscreenCheck())
		{
			Check->SetChecked(Save.Get<bool>(KEY_FULLSCREEN, false));
		}
		if (auto Check = UI.GetVSyncCheck())
		{
			Check->SetChecked(Save.Get<bool>(KEY_VSYNC, true));
		}
		if (auto Check = UI.GetSmoothRenderCheck())
		{
			Check->SetChecked(Save.Get<bool>(KEY_SMOOTH_RENDER, true));
		}

		// Gameplay settings
		if (auto Slider = UI.GetDeadzoneSlider())
		{
			float Value = Save.Get<float>(KEY_DEADZONE, 0.33f);
			Slider->SetValue(Value);
			if (auto Text = UI.GetDeadzoneValue())
				Text->SetText(ValueToPercentText(Value));
		}
		if (auto Slider = UI.GetCursorSpeedSlider())
		{
			float Value = Save.Get<float>(KEY_CURSOR_SPEED, 0.5f);
			Slider->SetValue(Value);
			if (auto Text = UI.GetCursorSpeedValue())
			{
				float Speed = 200.0f + Value * 1800.0f;
				Text->SetText(std::to_string(static_cast<int>(Speed)));
			}
		}

		LOG("SettingsController: Synced UI from AudioSubsystem");
	}

	void SettingsController::Show()
	{
		if (bIsVisible) return;
		bIsVisible = true;
		
		SyncUIFromSettings();
		UI.Show();
	}

	void SettingsController::Hide()
	{
		if (!bIsVisible) return;
		bIsVisible = false;
		UI.Hide();
	}

	bool SettingsController::IsVisible() const
	{
		return bIsVisible;
	}

	// =========================================================================
	// Audio Callbacks - Slider value is 0-1, AudioSubsystem expects 0-100
	// =========================================================================
	void SettingsController::OnMasterVolumeChanged(float SliderValue)
	{
		float Volume = SLIDER_TO_VOLUME(SliderValue); // 0-1 -> 0-100
		Subsystem.SaveLoad->Set<float>(KEY_MASTER_VOLUME, Volume);
		Subsystem.Audio->SetGlobalVolume(Volume);
		if (auto Text = UI.GetMasterValue())
			Text->SetText(ValueToPercentText(SliderValue));
		LOG("Master volume: {:.0f}% (saved & applied)", Volume);
	}

	void SettingsController::OnMusicVolumeChanged(float SliderValue)
	{
		float Volume = SLIDER_TO_VOLUME(SliderValue);
		Subsystem.SaveLoad->Set<float>(KEY_MUSIC_VOLUME, Volume);
		Subsystem.Audio->SetChannelVolume(AudioChannel::Music, Volume);
		if (auto Text = UI.GetMusicValue())
			Text->SetText(ValueToPercentText(SliderValue));
		LOG("Music volume: {:.0f}% (saved & applied)", Volume);
	}

	void SettingsController::OnAmbientVolumeChanged(float SliderValue)
	{
		float Volume = SLIDER_TO_VOLUME(SliderValue);
		Subsystem.SaveLoad->Set<float>(KEY_AMBIENT_VOLUME, Volume);
		Subsystem.Audio->SetChannelVolume(AudioChannel::Ambient, Volume);
		if (auto Text = UI.GetAmbientValue())
			Text->SetText(ValueToPercentText(SliderValue));
		LOG("Ambient volume: {:.0f}% (saved & applied)", Volume);
	}

	void SettingsController::OnSFXVolumeChanged(float SliderValue)
	{
		float Volume = SLIDER_TO_VOLUME(SliderValue);
		Subsystem.SaveLoad->Set<float>(KEY_SFX_VOLUME, Volume);
		Subsystem.Audio->SetChannelVolume(AudioChannel::SFX, Volume);
		if (auto Text = UI.GetSFXValue())
			Text->SetText(ValueToPercentText(SliderValue));
		LOG("SFX volume: {:.0f}% (saved & applied)", Volume);
	}

	// =========================================================================
	// Video Callbacks (save only, not yet hooked to subsystems)
	// =========================================================================
	void SettingsController::OnFullscreenToggled(bool bChecked)
	{
		Subsystem.SaveLoad->Set<bool>(KEY_FULLSCREEN, bChecked);
		LOG("Fullscreen: {} (saved)", bChecked ? "ON" : "OFF");
	}

	void SettingsController::OnVSyncToggled(bool bChecked)
	{
		Subsystem.SaveLoad->Set<bool>(KEY_VSYNC, bChecked);
		LOG("VSync: {} (saved)", bChecked ? "ON" : "OFF");
	}

	void SettingsController::OnSmoothRenderToggled(bool bChecked)
	{
		Subsystem.SaveLoad->Set<bool>(KEY_SMOOTH_RENDER, bChecked);
		LOG("Smooth Render: {} (saved)", bChecked ? "ON" : "OFF");
	}

	// =========================================================================
	// Gameplay Callbacks (save only, not yet hooked to subsystems)
	// =========================================================================
	void SettingsController::OnDeadzoneChanged(float Value)
	{
		Subsystem.SaveLoad->Set<float>(KEY_DEADZONE, Value);
		if (auto Text = UI.GetDeadzoneValue())
			Text->SetText(ValueToPercentText(Value));
		LOG("Deadzone: {:.0f}% (saved)", Value * 100.0f);
	}

	void SettingsController::OnCursorSpeedChanged(float Value)
	{
		Subsystem.SaveLoad->Set<float>(KEY_CURSOR_SPEED, Value);
		if (auto Text = UI.GetCursorSpeedValue())
		{
			float Speed = 200.0f + Value * 1800.0f;
			Text->SetText(std::to_string(static_cast<int>(Speed)));
		}
		LOG("Cursor Speed: {:.0f} (saved)", 200.0f + Value * 1800.0f);
	}

	// =========================================================================
	// Helpers
	// =========================================================================
	string SettingsController::ValueToPercentText(float Value)
	{
		return std::to_string(static_cast<int>(Value * 100.0f)) + "%";
	}
}

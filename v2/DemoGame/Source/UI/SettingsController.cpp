// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#include "UI/SettingsController.h"
#include "UI/SettingsMenuUI.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/SaveLoadSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	// Save keys
	constexpr stringView KEY_MASTER_VOLUME = "Settings.MasterVolume";
	constexpr stringView KEY_MUSIC_VOLUME = "Settings.MusicVolume";
	constexpr stringView KEY_AMBIENT_VOLUME = "Settings.AmbientVolume";
	constexpr stringView KEY_SFX_VOLUME = "Settings.SFXVolume";
	constexpr stringView KEY_VSYNC = "Settings.VSync";
	// Note: KEY_FULLSCREEN removed - fullscreen is runtime toggle only, not saved
	constexpr stringView KEY_SMOOTH_RENDER = "Settings.SmoothRender";
	constexpr stringView KEY_DEADZONE = "Settings.Deadzone";
	constexpr stringView KEY_CURSOR_SPEED = "Settings.CursorSpeed";

	// AudioSubsystem uses 0-100 range, sliders use 0-1 range
	constexpr float SLIDER_TO_VOLUME(float Slider) { return Slider * 100.0f; }
	constexpr float VOLUME_TO_SLIDER(float Volume) { return Volume / 100.0f; }
	constexpr float DEFAULT_VOLUME = 40.0f;  // 40% default for new players

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

		// UI bindings created
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

		// Video settings - read current values from WindowSubsystem
		// Note: Fullscreen is NOT saved - it's runtime-only to avoid startup issues
		if (auto Check = UI.GetFullscreenCheck())
		{
			Check->SetChecked(Subsystem.Window->IsFullscreen());
		}
		if (auto Check = UI.GetVSyncCheck())
		{
			bool bVSync = Save.Has(KEY_VSYNC)
				? Save.Get<bool>(KEY_VSYNC, true)
				: Subsystem.Window->IsVSync();
			Check->SetChecked(bVSync);
		}
		if (auto Check = UI.GetSmoothRenderCheck())
		{
			Check->SetChecked(Save.Get<bool>(KEY_SMOOTH_RENDER, true));
		}

		// Gameplay settings - read from CursorSubsystem (already set by DemoGameInstance)
		if (auto Slider = UI.GetDeadzoneSlider())
		{
			// Deadzone stored as 0-1 in CursorSubsystem, display as 0-100%
			float Deadzone = Subsystem.Cursor->GetJoystickDeadzone();
			float SliderValue = Deadzone; // Already 0-1
			Slider->SetValue(SliderValue);
			if (auto Text = UI.GetDeadzoneValue())
				Text->SetText(ValueToPercentText(SliderValue));
		}
		if (auto Slider = UI.GetCursorSpeedSlider())
		{
			// Speed stored as 200-2000 in CursorSubsystem, convert to 0-1 slider
			float Speed = Subsystem.Cursor->GetSpeed();
			float SliderValue = (Speed - 200.0f) / 1800.0f;
			Slider->SetValue(SliderValue);
			if (auto Text = UI.GetCursorSpeedValue())
				Text->SetText(std::to_string(static_cast<int>(Speed)));
		}

		// UI synced from AudioSubsystem
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
	}

	void SettingsController::OnMusicVolumeChanged(float SliderValue)
	{
		float Volume = SLIDER_TO_VOLUME(SliderValue);
		Subsystem.SaveLoad->Set<float>(KEY_MUSIC_VOLUME, Volume);
		Subsystem.Audio->SetChannelVolume(AudioChannel::Music, Volume);
		if (auto Text = UI.GetMusicValue())
			Text->SetText(ValueToPercentText(SliderValue));
	}

	void SettingsController::OnAmbientVolumeChanged(float SliderValue)
	{
		float Volume = SLIDER_TO_VOLUME(SliderValue);
		Subsystem.SaveLoad->Set<float>(KEY_AMBIENT_VOLUME, Volume);
		Subsystem.Audio->SetChannelVolume(AudioChannel::Ambient, Volume);
		if (auto Text = UI.GetAmbientValue())
			Text->SetText(ValueToPercentText(SliderValue));
	}

	void SettingsController::OnSFXVolumeChanged(float SliderValue)
	{
		float Volume = SLIDER_TO_VOLUME(SliderValue);
		Subsystem.SaveLoad->Set<float>(KEY_SFX_VOLUME, Volume);
		Subsystem.Audio->SetChannelVolume(AudioChannel::SFX, Volume);
		if (auto Text = UI.GetSFXValue())
			Text->SetText(ValueToPercentText(SliderValue));
	}

	// =========================================================================
	// Video Callbacks - Immediate application to WindowSubsystem
	// =========================================================================
	void SettingsController::OnFullscreenToggled(bool bChecked)
	{
		// Note: Fullscreen is NOT saved - toggle is runtime-only to avoid startup issues
		Subsystem.Window->SetFullscreen(bChecked);
	}

	void SettingsController::OnVSyncToggled(bool bChecked)
	{
		Subsystem.SaveLoad->Set<bool>(KEY_VSYNC, bChecked);
		Subsystem.Window->SetVSync(bChecked);
	}

	void SettingsController::OnSmoothRenderToggled(bool bChecked)
	{
		Subsystem.SaveLoad->Set<bool>(KEY_SMOOTH_RENDER, bChecked);
	}

	// =========================================================================
	// Gameplay Callbacks - Immediate application to CursorSubsystem
	// =========================================================================
	void SettingsController::OnDeadzoneChanged(float SliderValue)
	{
		// Slider 0-1 maps directly to deadzone 0-1
		Subsystem.SaveLoad->Set<float>(KEY_DEADZONE, SliderValue);
		Subsystem.Cursor->SetJoystickDeadzone(SliderValue);
		if (auto Text = UI.GetDeadzoneValue())
			Text->SetText(ValueToPercentText(SliderValue));
	}

	void SettingsController::OnCursorSpeedChanged(float SliderValue)
	{
		// Slider 0-1 -> Speed 200-2000
		float Speed = 200.0f + SliderValue * 1800.0f;
		Subsystem.SaveLoad->Set<float>(KEY_CURSOR_SPEED, Speed);
		Subsystem.Cursor->SetSpeed(Speed);
		if (auto Text = UI.GetCursorSpeedValue())
			Text->SetText(std::to_string(static_cast<int>(Speed)));
	}

	// =========================================================================
	// Helpers
	// =========================================================================
	string SettingsController::ValueToPercentText(float Value)
	{
		return std::to_string(static_cast<int>(Value * 100.0f)) + "%";
	}
}

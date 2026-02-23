// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#include "GameInstance/DemoGameInstance.h"
#include "UI/SettingsMenuUI.h"
#include "UI/SettingsController.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/SaveLoadSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "GameConfig.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"

namespace we
{
	// Save keys (must match SettingsController)
	constexpr stringView KEY_MASTER_VOLUME = "Settings.MasterVolume";
	constexpr stringView KEY_MUSIC_VOLUME = "Settings.MusicVolume";
	constexpr stringView KEY_AMBIENT_VOLUME = "Settings.AmbientVolume";
	constexpr stringView KEY_SFX_VOLUME = "Settings.SFXVolume";
	constexpr stringView KEY_VSYNC = "Settings.VSync";
	constexpr stringView KEY_DEADZONE = "Settings.Deadzone";
	constexpr stringView KEY_CURSOR_SPEED = "Settings.CursorSpeed";
	// Note: Fullscreen is NOT saved - runtime toggle only

	DemoGameInstance::DemoGameInstance()
	{
	}

	DemoGameInstance::~DemoGameInstance()
	{
		// DemoGameInstance destroyed
	}

	void DemoGameInstance::Init(EngineSubsystem& InSubsystem)
	{
		GameInstance::Init(InSubsystem);
		
		// DemoGameInstance initializing...
		
		// Apply saved VSync setting (safe to do during init)
		ApplySavedVSyncSetting();
		
		// Apply saved gameplay settings (cursor/deadzone)
		ApplySavedGameplaySettings();
		
		// Apply saved audio settings BEFORE any music starts playing
		ApplySavedAudioSettings();
		
		// Start playing default music and ambient (looping)
		Subsystem->Audio->PlayMusic(GC.DefaultMusic, true);
		Subsystem->Audio->PlayAmbient(GC.DefaultAmbient, true);
		LOG("Started music: {}", GC.DefaultMusic);
		LOG("Started ambient: {}", GC.DefaultAmbient);
		
		// Create persistent settings menu (hidden by default)
		SettingsUI = make_unique<SettingsMenuUI>(*Subsystem);
		SettingsCtrl = make_unique<SettingsController>(*Subsystem, *SettingsUI);
		SettingsCtrl->BindUI();
		
		// Bind back button
		SettingsUI->OnBackClicked.Bind(this, &DemoGameInstance::OnSettingsBackClicked);
		
		// Bind GUI navigation input (gamepad)
		BindGUINavigationInput();
		
		// DemoGameInstance initialized
	}

	void DemoGameInstance::ApplySavedVSyncSetting()
	{
		auto& Save = *Subsystem->SaveLoad;
		auto& Window = *Subsystem->Window;
		
		// Apply saved VSync setting if it exists
		if (Save.Has(KEY_VSYNC))
		{
			bool bVSync = Save.Get<bool>(KEY_VSYNC, true);
			Window.SetVSync(bVSync);
			LOG("Applied saved VSync: {}", bVSync ? "ON" : "OFF");
		}
	}

	void DemoGameInstance::ApplySavedGameplaySettings()
	{
		auto& Save = *Subsystem->SaveLoad;
		auto& Cursor = *Subsystem->Cursor;
		
		// Apply saved deadzone if it exists (stored as 0-1)
		if (Save.Has(KEY_DEADZONE))
		{
			float Deadzone = Save.Get<float>(KEY_DEADZONE, 0.33f);
			Cursor.SetJoystickDeadzone(Deadzone);
			LOG("Applied saved joystick deadzone: {:.0f}%", Deadzone * 100.0f);
		}
		
		// Apply saved cursor speed if it exists (stored as 200-2000)
		if (Save.Has(KEY_CURSOR_SPEED))
		{
			float Speed = Save.Get<float>(KEY_CURSOR_SPEED, 1000.0f);
			Cursor.SetSpeed(Speed);
			LOG("Applied saved cursor speed: {:.0f}", Speed);
		}
	}

	void DemoGameInstance::ApplySavedAudioSettings()
	{
		auto& Save = *Subsystem->SaveLoad;
		auto& Audio = *Subsystem->Audio;
		
		// Default volume for new players (40%)
		constexpr float DEFAULT_VOL = 40.0f;
		
		// Apply saved audio settings, or set defaults for new players
		// This ensures music/ambient that starts playing uses the correct volume
		float MasterVol = Save.Get<float>(KEY_MASTER_VOLUME, DEFAULT_VOL);
		Audio.SetGlobalVolume(MasterVol);
		
		float MusicVol = Save.Get<float>(KEY_MUSIC_VOLUME, DEFAULT_VOL);
		Audio.SetChannelVolume(AudioChannel::Music, MusicVol);
		
		float AmbientVol = Save.Get<float>(KEY_AMBIENT_VOLUME, DEFAULT_VOL);
		Audio.SetChannelVolume(AudioChannel::Ambient, AmbientVol);
		
		float SFXVol = Save.Get<float>(KEY_SFX_VOLUME, DEFAULT_VOL);
		Audio.SetChannelVolume(AudioChannel::SFX, SFXVol);
		
		if (Save.Has(KEY_MASTER_VOLUME))
		{
			LOG("Applied saved audio settings from SaveLoad");
		}
		else
		{
			LOG("No save file found - using default 40% volume for new player");
		}
	}

	void DemoGameInstance::Shutdown()
	{
		// DemoGameInstance shutting down
		
		SettingsCtrl.reset();
		SettingsUI.reset();
		
		GameInstance::Shutdown();
	}

	void DemoGameInstance::Tick(float DeltaTime)
	{
		GameInstance::Tick(DeltaTime);
	}

	void DemoGameInstance::ShowSettings()
	{
		if (SettingsCtrl)
		{
			// Showing settings menu
			SettingsCtrl->Show();
		}
	}

	void DemoGameInstance::HideSettings()
	{
		if (SettingsCtrl)
		{
			// Hiding settings menu
			SettingsCtrl->Hide();
		}
	}

	bool DemoGameInstance::IsSettingsVisible() const
	{
		return SettingsCtrl ? SettingsCtrl->IsVisible() : false;
	}

	void DemoGameInstance::OnSettingsBackClicked()
	{
		// Settings closed (back button clicked)
		HideSettings();
		
		// Notify listeners and clear the delegate to prevent stale bindings
		OnSettingsClosed.Broadcast();
		OnSettingsClosed.Clear();
	}

	void DemoGameInstance::BindGUINavigationInput()
	{
		using enum GamepadButton;
		auto& Input = *Subsystem->Input;
		auto& GUI = *Subsystem->GUI;

		// Bind hardware inputs to GUI actions
		Input.Bind(GUI_CONFIRM, Input::Gamepad{ South, 0 });
		Input.Bind(GUI_CANCEL, Input::Gamepad{ East, 0 });
		Input.Bind(GUI_NAV_NEXT, Input::Gamepad{ R1, 0 });
		Input.Bind(GUI_NAV_PREVIOUS, Input::Gamepad{ L1, 0 });

		// Bind actions to GUI subsystem methods
		Input.OnPressed(GUI_CONFIRM, [&GUI]() { GUI.OnGamepadConfirmPressed(); });
		Input.OnReleased(GUI_CONFIRM, [&GUI]() { GUI.OnGamepadConfirmReleased(); });
		Input.OnPressed(GUI_CANCEL, [&GUI]() { GUI.OnGamepadCancel(); });
		Input.OnPressed(GUI_NAV_NEXT, [&GUI]() { GUI.OnGamepadNavigateNext(); });
		Input.OnPressed(GUI_NAV_PREVIOUS, [&GUI]() { GUI.OnGamepadNavigatePrevious(); });

		LOG("GUI navigation input bound");
	}
}

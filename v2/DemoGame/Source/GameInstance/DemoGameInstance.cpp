// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#include "GameInstance/DemoGameInstance.h"
#include "UI/SettingsMenuUI.h"
#include "UI/SettingsController.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/SaveLoadSubsystem.h"
#include "GameConfig.h"
#include "Utility/Log.h"

namespace we
{
	// Save keys (must match SettingsController)
	constexpr stringView KEY_MASTER_VOLUME = "Settings.MasterVolume";
	constexpr stringView KEY_MUSIC_VOLUME = "Settings.MusicVolume";
	constexpr stringView KEY_AMBIENT_VOLUME = "Settings.AmbientVolume";
	constexpr stringView KEY_SFX_VOLUME = "Settings.SFXVolume";

	DemoGameInstance::DemoGameInstance()
	{
		LOG("DemoGameInstance created");
	}

	DemoGameInstance::~DemoGameInstance()
	{
		LOG("DemoGameInstance destroyed");
	}

	void DemoGameInstance::Init(EngineSubsystem& InSubsystem)
	{
		GameInstance::Init(InSubsystem);
		
		LOG("DemoGameInstance initializing persistent systems...");
		
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
		
		LOG("DemoGameInstance initialized");
	}

	void DemoGameInstance::ApplySavedAudioSettings()
	{
		auto& Save = *Subsystem->SaveLoad;
		auto& Audio = *Subsystem->Audio;
		
		// If we have saved audio settings, apply them to AudioSubsystem immediately
		// This ensures music/ambient that starts playing uses the correct volume
		if (Save.Has(KEY_MASTER_VOLUME))
		{
			float MasterVol = Save.Get<float>(KEY_MASTER_VOLUME, 0.5f);
			Audio.SetGlobalVolume(MasterVol);
			LOG("Applied saved master volume: {:.0f}%", MasterVol * 100.0f);
		}
		if (Save.Has(KEY_MUSIC_VOLUME))
		{
			float MusicVol = Save.Get<float>(KEY_MUSIC_VOLUME, 0.5f);
			Audio.SetChannelVolume(AudioChannel::Music, MusicVol);
			LOG("Applied saved music volume: {:.0f}%", MusicVol * 100.0f);
		}
		if (Save.Has(KEY_AMBIENT_VOLUME))
		{
			float AmbientVol = Save.Get<float>(KEY_AMBIENT_VOLUME, 0.5f);
			Audio.SetChannelVolume(AudioChannel::Ambient, AmbientVol);
			LOG("Applied saved ambient volume: {:.0f}%", AmbientVol * 100.0f);
		}
		if (Save.Has(KEY_SFX_VOLUME))
		{
			float SFXVol = Save.Get<float>(KEY_SFX_VOLUME, 0.5f);
			Audio.SetChannelVolume(AudioChannel::SFX, SFXVol);
			LOG("Applied saved SFX volume: {:.0f}%", SFXVol * 100.0f);
		}
	}

	void DemoGameInstance::Shutdown()
	{
		LOG("DemoGameInstance shutting down");
		
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
			LOG("DemoGameInstance: Showing settings menu");
			SettingsCtrl->Show();
		}
	}

	void DemoGameInstance::HideSettings()
	{
		if (SettingsCtrl)
		{
			LOG("DemoGameInstance: Hiding settings menu");
			SettingsCtrl->Hide();
		}
	}

	bool DemoGameInstance::IsSettingsVisible() const
	{
		return SettingsCtrl ? SettingsCtrl->IsVisible() : false;
	}

	void DemoGameInstance::OnSettingsBackClicked()
	{
		LOG("DemoGameInstance: Settings back clicked");
		HideSettings();
		OnSettingsClosed.Broadcast();
	}
}

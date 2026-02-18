// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/GameInstance.h"
#include "Utility/Delegate.h"

namespace we
{
	class SettingsMenuUI;
	class SettingsController;

	// =============================================================================
	// DemoGameInstance
	// =============================================================================
	// Game-specific persistent data and systems.
	// Lives for the entire game session.
	// =============================================================================
	
	class DemoGameInstance : public GameInstance
	{
	public:
		DemoGameInstance();
		~DemoGameInstance() override;

		// GameInstance interface
		void Init(EngineSubsystem& InSubsystem) override;
		void Shutdown() override;
		void Tick(float DeltaTime) override;

		// Settings Menu - accessible from anywhere (MainMenu, Pause, etc.)
		void ShowSettings();
		void HideSettings();
		bool IsSettingsVisible() const;

		// Bind this to know when user clicks "Back" in settings
		Delegate<> OnSettingsClosed;

	private:
		void ApplySavedAudioSettings();
		void OnSettingsBackClicked();

	private:
		// Persistent UI - created once, shown/hidden as needed
		unique<SettingsMenuUI> SettingsUI;
		unique<SettingsController> SettingsCtrl;
	};
}

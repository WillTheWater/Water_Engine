// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/WidgetController.h"

namespace we
{
	class SettingsMenuUI;

	// =========================================================================
	// SettingsController - Controls the settings menu
	// =========================================================================
	class SettingsController : public WidgetController
	{
	public:
		SettingsController(EngineSubsystem& InSubsystem, SettingsMenuUI& InUI);

		// WidgetController interface
		virtual void BindUI() override;
		virtual void SyncUIFromSettings() override;
		virtual void Show() override;
		virtual void Hide() override;
		virtual bool IsVisible() const override;

	private:
		// Bound widget callbacks
		void OnMasterVolumeChanged(float Value);
		void OnMusicVolumeChanged(float Value);
		void OnAmbientVolumeChanged(float Value);
		void OnSFXVolumeChanged(float Value);

		void OnFullscreenToggled(bool bChecked);
		void OnVSyncToggled(bool bChecked);
		void OnSmoothRenderToggled(bool bChecked);

		void OnDeadzoneChanged(float Value);
		void OnCursorSpeedChanged(float Value);

		static string ValueToPercentText(float Value);

	private:
		SettingsMenuUI& UI;
	};
}

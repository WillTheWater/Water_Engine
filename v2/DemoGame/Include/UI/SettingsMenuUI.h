// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Utility/Delegate.h"

namespace we
{
	class Panel;
	class Slider;
	class CheckBox;
	class Button;
	class TextBlock;
	struct EngineSubsystem;

	class SettingsMenuUI
	{
	public:
		explicit SettingsMenuUI(EngineSubsystem& InSubsystem);
		~SettingsMenuUI();

		void Show();
		void Hide();
		bool IsVisible() const { return bIsVisible; }

		Delegate<> OnBackClicked;

		// Accessors for controller
		shared<Slider> GetMasterSlider() const { return MasterSlider; }
		shared<Slider> GetMusicSlider() const { return MusicSlider; }
		shared<Slider> GetAmbientSlider() const { return AmbientSlider; }
		shared<Slider> GetSFXSlider() const { return SFXSlider; }
		shared<Slider> GetDeadzoneSlider() const { return DeadzoneSlider; }
		shared<Slider> GetCursorSpeedSlider() const { return CursorSpeedSlider; }

		shared<CheckBox> GetFullscreenCheck() const { return FullscreenCheck; }
		shared<CheckBox> GetVSyncCheck() const { return VSyncCheck; }
		shared<CheckBox> GetSmoothRenderCheck() const { return SmoothRenderCheck; }

		shared<TextBlock> GetMasterValue() const { return MasterValue; }
		shared<TextBlock> GetMusicValue() const { return MusicValue; }
		shared<TextBlock> GetAmbientValue() const { return AmbientValue; }
		shared<TextBlock> GetSFXValue() const { return SFXValue; }
		shared<TextBlock> GetDeadzoneValue() const { return DeadzoneValue; }
		shared<TextBlock> GetCursorSpeedValue() const { return CursorSpeedValue; }

	private:
		void CreateUI();
		void CreateSectionHeader(const string& Text, float YOffset);
		void CreateSliderRow(const string& Label, float YOffset, shared<Slider>& OutSlider, shared<TextBlock>& OutValue, float InitialValue);
		void CreateCheckBoxRow(const string& Label, float YOffset, shared<CheckBox>& OutCheckBox, bool bInitialState);

	private:
		EngineSubsystem& Subsystem;
		bool bIsVisible;

		// Main container
		shared<Panel> BackgroundPanel;

		// References to interactive widgets for logic binding
		shared<Slider> MasterSlider;
		shared<Slider> MusicSlider;
		shared<Slider> AmbientSlider;
		shared<Slider> SFXSlider;
		shared<Slider> DeadzoneSlider;
		shared<Slider> CursorSpeedSlider;

		shared<CheckBox> FullscreenCheck;
		shared<CheckBox> VSyncCheck;
		shared<CheckBox> SmoothRenderCheck;

		shared<TextBlock> MasterValue;
		shared<TextBlock> MusicValue;
		shared<TextBlock> AmbientValue;
		shared<TextBlock> SFXValue;
		shared<TextBlock> DeadzoneValue;
		shared<TextBlock> CursorSpeedValue;

		shared<Button> BackButton;
	};
}

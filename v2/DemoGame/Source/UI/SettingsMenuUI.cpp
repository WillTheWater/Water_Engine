// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/SettingsMenuUI.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "UI/Widget/Panel.h"
#include "UI/Widget/TextBlock.h"
#include "UI/Widget/Slider.h"
#include "UI/Widget/CheckBox.h"
#include "UI/Widget/Button.h"
#include "EngineConfig.h"

namespace we
{
	SettingsMenuUI::SettingsMenuUI(EngineSubsystem& InSubsystem)
		: Subsystem(InSubsystem)
		, bIsVisible(false)
	{
		CreateUI();
	}

	SettingsMenuUI::~SettingsMenuUI() = default;

	void SettingsMenuUI::Show()
	{
		if (bIsVisible) return;
		bIsVisible = true;
		if (BackgroundPanel) BackgroundPanel->SetVisible(true);
	}

	void SettingsMenuUI::Hide()
	{
		if (!bIsVisible) return;
		bIsVisible = false;
		if (BackgroundPanel) BackgroundPanel->SetVisible(false);
	}

	void SettingsMenuUI::CreateUI()
	{
		vec2f Center = vec2f(EC.RenderResolution) / 2.0f;

		// Main background panel
		BackgroundPanel = Subsystem.GUI->CreatePanel({ 700.0f, 800.0f }, color{ 30, 30, 30, 240 }, color{ 100, 100, 100 }, 2.0f);
		BackgroundPanel->SetAnchorPosition(Anchor::Center, Anchor::Center);
		BackgroundPanel->SetRenderDepth(200.0f);
		BackgroundPanel->SetVisible(false);

		// Title
		auto Title = Subsystem.GUI->CreateTextBlock("SETTINGS", 0.0f, 36);
		Title->SetColor(color::White);
		Title->SetAlignment(TextAlignment::Center);
		BackgroundPanel->AddChild(Title, Anchor::TopCenter, Anchor::TopCenter, { 0.0f, 20.0f });

		// ========== AUDIO SECTION ==========
		float CurrentY = 80.0f;
		CreateSectionHeader("AUDIO", CurrentY);
		CurrentY += 45.0f;

		CreateSliderRow("Master", CurrentY, MasterSlider, MasterValue, 0.5f);
		CurrentY += 50.0f;
		CreateSliderRow("Music", CurrentY, MusicSlider, MusicValue, 0.5f);
		CurrentY += 50.0f;
		CreateSliderRow("Ambient", CurrentY, AmbientSlider, AmbientValue, 0.5f);
		CurrentY += 50.0f;
		CreateSliderRow("SFX", CurrentY, SFXSlider, SFXValue, 0.5f);
		CurrentY += 70.0f;

		// ========== VIDEO SECTION ==========
		CreateSectionHeader("VIDEO", CurrentY);
		CurrentY += 45.0f;

		CreateCheckBoxRow("Fullscreen", CurrentY, FullscreenCheck, false);
		CurrentY += 50.0f;
		CreateCheckBoxRow("VSync", CurrentY, VSyncCheck, true);
		CurrentY += 50.0f;
		CreateCheckBoxRow("Smooth Render", CurrentY, SmoothRenderCheck, true);
		CurrentY += 70.0f;

		// ========== GAMEPLAY SECTION ==========
		CreateSectionHeader("GAMEPLAY", CurrentY);
		CurrentY += 45.0f;

		CreateSliderRow("Joy Deadzone", CurrentY, DeadzoneSlider, DeadzoneValue, 0.33f);
		CurrentY += 50.0f;
		CreateSliderRow("Cursor Speed", CurrentY, CursorSpeedSlider, CursorSpeedValue, 0.5f);
		CurrentY += 70.0f;

		// Back Button
		BackButton = Subsystem.GUI->CreateButton("BACK", { 200.0f, 50.0f }, color{ 100, 100, 100 }, color::White, 2.0f);
		BackgroundPanel->AddChild(BackButton, Anchor::TopCenter, Anchor::TopCenter, { 0.0f, CurrentY });
		BackButton->OnClicked.Bind([this]() {
			Hide();
			OnBackClicked.Broadcast();
		});
	}

	void SettingsMenuUI::CreateSectionHeader(const string& Text, float YOffset)
	{
		auto Header = Subsystem.GUI->CreateTextBlock(Text, 0.0f, 26);
		Header->SetColor(color{ 100, 200, 255 });
		BackgroundPanel->AddChild(Header, Anchor::TopLeft, Anchor::TopLeft, { 20.0f, YOffset });
	}

	void SettingsMenuUI::CreateSliderRow(const string& Label, float YOffset, shared<Slider>& OutSlider, shared<TextBlock>& OutValue, float InitialValue)
	{
		// Label
		auto LabelText = Subsystem.GUI->CreateTextBlock(Label, 0.0f, 20);
		LabelText->SetColor(color::White);
		BackgroundPanel->AddChild(LabelText, Anchor::TopLeft, Anchor::TopLeft, { 20.0f, YOffset });

		// Slider
		OutSlider = Subsystem.GUI->CreateSlider(250.0f, 10.0f, 20.0f, 25.0f);
		OutSlider->SetValue(InitialValue);
		BackgroundPanel->AddChild(OutSlider, Anchor::TopLeft, Anchor::TopLeft, { 200.0f, YOffset + 5.0f });

		// Value text
		OutValue = Subsystem.GUI->CreateTextBlock("50%", 0.0f, 20);
		OutValue->SetColor(color::White);
		BackgroundPanel->AddChild(OutValue, Anchor::TopLeft, Anchor::TopLeft, { 470.0f, YOffset });

		// Bind value change to update text
		OutSlider->OnValueChanged.Bind([OutValue](float Value) {
			int Percent = static_cast<int>(Value * 100.0f);
			OutValue->SetText(std::to_string(Percent) + "%");
		});

		// Set initial value
		int InitialPercent = static_cast<int>(InitialValue * 100.0f);
		OutValue->SetText(std::to_string(InitialPercent) + "%");
	}

	void SettingsMenuUI::CreateCheckBoxRow(const string& Label, float YOffset, shared<CheckBox>& OutCheckBox, bool bInitialState)
	{
		// Label
		auto LabelText = Subsystem.GUI->CreateTextBlock(Label, 0.0f, 20);
		LabelText->SetColor(color::White);
		BackgroundPanel->AddChild(LabelText, Anchor::TopLeft, Anchor::TopLeft, { 20.0f, YOffset });

		// Checkbox
		OutCheckBox = Subsystem.GUI->CreateCheckBox(bInitialState, 24.0f, CheckBoxShape::Rectangle);
		BackgroundPanel->AddChild(OutCheckBox, Anchor::TopLeft, Anchor::TopLeft, { 470.0f, YOffset + 5.0f });
	}
}

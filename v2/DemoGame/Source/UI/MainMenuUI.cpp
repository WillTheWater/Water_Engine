// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/MainMenuUI.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/Panel.h"
#include "UI/Widget/VerticalBox.h"
#include "UI/Widget/HorizontalBox.h"
#include "UI/Widget/GridBox.h"
#include "UI/Widget/AutoPanel.h"
#include "UI/Widget/Spacer.h"
#include "UI/Widget/CheckBox.h"
#include "UI/Widget/Slider.h"
#include "UI/Widget/ProgressBar.h"
#include "UI/Widget/TextBlock.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	MainMenuUI::MainMenuUI(EngineSubsystem& InSubsystem)
		: Subsystem{ InSubsystem }
	{
		CreateUI();
	}

	MainMenuUI::~MainMenuUI()
	{
	}

	void MainMenuUI::CreateUI()
	{
		MenuPanel = Subsystem.GUI->CreatePanel({ 400.f, 300.f }, color::White, color::Black, 4.f);
		MenuPanel->SetAnchorPosition(Anchor::Center, Anchor::Center);
		MenuPanel->SetRenderDepth(50.f);

		ColorButton = Subsystem.GUI->CreateButton("Color Button", vec2f{ 200.f, 50.f });
		ColorButton->SetRenderDepth(100.f);
		ColorButton->SetHoverSound(GC.DefaultButtonHoverSound);
		ColorButton->SetClickSound(GC.DefaultButtonClickSound);
		ColorButton->OnClicked.Bind(this, &MainMenuUI::OnColorButtonClicked);
		MenuPanel->AddChild(ColorButton, Anchor::Center, Anchor::Center, { 0.f, -40.f });

		TextureButton = Subsystem.GUI->CreateTextureButton("Texture Button", GC.DefaultButton, { 200.f, 50.f });
		TextureButton->SetRenderDepth(100.f);
		TextureButton->SetHoverSound(GC.DefaultButtonHoverSound);
		TextureButton->SetClickSound(GC.DefaultButtonClickSound);
		TextureButton->OnClicked.Bind(this, &MainMenuUI::OnTextureButtonClicked);
		MenuPanel->AddChild(TextureButton, Anchor::Center, Anchor::Center, { 0.f, 40.f });

		// Test VerticalBox - Top Left
		auto VBox = Subsystem.GUI->CreateVerticalBox(15.f);
		VBox->SetAnchorPosition(Anchor::TopLeft, Anchor::TopLeft, { 20.f, 20.f });
		VBox->SetRenderDepth(60.f);

		auto VB1 = Subsystem.GUI->CreateButton("V1", { 80.f, 40.f }, color{ 100, 200, 100 });
		auto VB2 = Subsystem.GUI->CreateButton("V2", { 100.f, 40.f }, color{ 100, 200, 100 });
		auto VB3 = Subsystem.GUI->CreateButton("V3", { 120.f, 40.f }, color{ 100, 200, 100 });
		VBox->AddChild(VB1);
		VBox->AddChild(VB2);
		VBox->AddChild(VB3);

		// Test HorizontalBox - Top Right
		auto HBox = Subsystem.GUI->CreateHorizontalBox(15.f);
		HBox->SetAnchorPosition(Anchor::TopRight, Anchor::TopRight, { -20.f, 20.f });
		HBox->SetRenderDepth(60.f);

		auto HB1 = Subsystem.GUI->CreateButton("H1", { 60.f, 60.f }, color{ 200, 100, 100 });
		auto HB2 = Subsystem.GUI->CreateButton("H2", { 60.f, 50.f }, color{ 200, 100, 100 });
		auto HB3 = Subsystem.GUI->CreateButton("H3", { 60.f, 70.f }, color{ 200, 100, 100 });
		HBox->AddChild(HB1);
		HBox->AddChild(HB2);
		HBox->AddChild(HB3);

		// Test TextBlocks - Center Left
		// Left aligned wrapped text
		auto TextLeft = Subsystem.GUI->CreateTextBlock(
			"This is left aligned text that wraps automatically when it gets too long.",
			200.f,  // wrap width
			18);
		TextLeft->SetRenderDepth(100.f);
		TextLeft->SetAlignment(TextAlignment::Left);
		TextLeft->SetColor(color::White);
		TextLeft->SetOutlineColor(color::Black);
		TextLeft->SetOutlineThickness(1.5f);
		TextLeft->SetAnchorPosition(Anchor::CenterLeft, Anchor::CenterLeft, { 400.f, -60.f });

		// Center aligned wrapped text
		auto TextCenter = Subsystem.GUI->CreateTextBlock(
			"Center aligned text stays centered within the wrap width.",
			200.f,
			18);
		TextCenter->SetRenderDepth(100.f);
		TextCenter->SetAlignment(TextAlignment::Center);
		TextCenter->SetColor(color{ 100, 200, 100 });
		TextCenter->SetAnchorPosition(Anchor::CenterLeft, Anchor::CenterLeft, { 20.f, 0.f });

		// Right aligned wrapped text with outline
		auto TextRight = Subsystem.GUI->CreateTextBlock(
			"Right aligned text with outline and custom line spacing.",
			200.f,
			18);
		TextRight->SetRenderDepth(100.f);
		TextRight->SetAlignment(TextAlignment::Right);
		TextRight->SetColor(color{ 200, 150, 100 });
		TextRight->SetOutlineColor(color::Black);
		TextRight->SetOutlineThickness(1.5f);
		TextRight->SetLineSpacing(2.5f);
		TextRight->SetAnchorPosition(Anchor::CenterLeft, Anchor::CenterLeft, { 200.f, 200.f });

		// Test GridBox - Bottom Left
		auto GBox = Subsystem.GUI->CreateGridBox(2, 10.f);
		GBox->SetAnchorPosition(Anchor::BottomLeft, Anchor::BottomLeft, { 20.f, -20.f });
		GBox->SetRenderDepth(60.f);

		auto GB1 = Subsystem.GUI->CreateButton("G1", { 80.f, 40.f }, color{ 100, 100, 200 });
		auto GB2 = Subsystem.GUI->CreateButton("G2", { 80.f, 40.f }, color{ 100, 100, 200 });
		auto GB3 = Subsystem.GUI->CreateButton("G3", { 80.f, 40.f }, color{ 100, 100, 200 });
		auto GB4 = Subsystem.GUI->CreateButton("G4", { 80.f, 40.f }, color{ 100, 100, 200 });
		auto GB5 = Subsystem.GUI->CreateButton("G5", { 80.f, 40.f }, color{ 100, 100, 200 });
		auto GB6 = Subsystem.GUI->CreateButton("G6", { 80.f, 40.f }, color{ 100, 100, 200 });
		GBox->AddChild(GB1);
		GBox->AddChild(GB2);
		GBox->AddChild(GB3);
		GBox->AddChild(GB4);
		GBox->AddChild(GB5);
		GBox->AddChild(GB6);

		// Test AutoPanel with different alignments - Bottom Right
		auto AutoP = Subsystem.GUI->CreateAutoPanel(color{ 0, 0, 0, 128 }, color::White, 4.f, 10.f);
		AutoP->SetAnchorPosition(Anchor::BottomRight, Anchor::BottomRight, { -300.f, -50.f });
		AutoP->SetRenderDepth(70.f);
		AutoP->SetBorder(20.f, 20.f, 15.f, 150.f);

		auto AP1 = Subsystem.GUI->CreateButton("Left", { 100.f, 30.f }, color{ 200, 100, 100 });
		AP1->SetRenderDepth(75.f);
		auto AP2 = Subsystem.GUI->CreateButton("Center", { 120.f, 30.f }, color{ 100, 200, 100 });
		AP2->SetRenderDepth(75.f);
		auto AP3 = Subsystem.GUI->CreateButton("Right", { 100.f, 30.f }, color{ 100, 100, 200 });
		AP3->SetRenderDepth(75.f);
		AutoP->AddChild(AP1, ChildAlignment::Left);
		AutoP->AddChild(AP2, ChildAlignment::Center);
		AutoP->AddChild(AP3, ChildAlignment::Right);

		// Add spacer between buttons for spacing test
		auto Spacer1 = Subsystem.GUI->CreateSpacer(10.f, 5.f);
		Spacer1->SetRenderDepth(175.f);
		AutoP->AddChild(Spacer1, ChildAlignment::Left);

		auto AP4 = Subsystem.GUI->CreateButton("After Spacer", { 200.f, 40.f }, color{ 200, 200, 100 });
		AP4->SetRenderDepth(75.f);
		AutoP->AddChild(AP4, ChildAlignment::Center);

		// Add checkboxes to AutoPanel (no labels)
		auto Check1 = Subsystem.GUI->CreateCheckBox(false, 50.f, CheckBoxShape::Rectangle);
		Check1->SetRenderDepth(75.f);
		Check1->OnToggled.Bind([](bool bChecked) { LOG("Rect CheckBox: {}", bChecked); });
		AutoP->AddChild(Check1, ChildAlignment::Left);

		auto Check2 = Subsystem.GUI->CreateCheckBox(true, 50.f, CheckBoxShape::Circle);
		Check2->SetRenderDepth(75.f);
		Check2->OnToggled.Bind([](bool bChecked) { LOG("Circle CheckBox: {}", bChecked); });
		AutoP->AddChild(Check2, ChildAlignment::Left);

		auto Check3 = Subsystem.GUI->CreateCheckBox(false, 50.f, CheckBoxShape::Rectangle);
		Check3->SetRenderDepth(75.f);
		Check3->SetNormalColor(color{ 100, 100, 100 });
		Check3->SetCheckedColor(color{ 100, 100, 200 });
		Check3->OnToggled.Bind([](bool bChecked) { LOG("Custom Color CheckBox: {}", bChecked); });
		AutoP->AddChild(Check3, ChildAlignment::Left);

		// Add spacer before sliders
		auto Spacer2 = Subsystem.GUI->CreateSpacer(10.f, 15.f);
		Spacer2->SetRenderDepth(175.f);
		AutoP->AddChild(Spacer2, ChildAlignment::Left);

		// Test Sliders
		// Horizontal rectangle slider
		auto Slider1 = Subsystem.GUI->CreateSlider(180.f, 10.f, 20.f, 30.f, SliderOrientation::Horizontal);
		Slider1->SetRenderDepth(75.f);
		Slider1->SetValue(0.3f);
		Slider1->OnValueChanged.Bind([](float Value) { LOG("Slider1 (H): {:.2f}", Value); });
		AutoP->AddChild(Slider1, ChildAlignment::Left);

		// Vertical rectangle slider
		auto Slider2 = Subsystem.GUI->CreateSlider(30.f, 150.f, 40.f, 20.f, SliderOrientation::Vertical);
		Slider2->SetRenderDepth(75.f);
		Slider2->SetValue(0.7f);
		Slider2->SetTrackColor(color{ 200, 100, 100 });
		Slider2->SetThumbColor(color{ 100, 200, 100 });
		Slider2->OnValueChanged.Bind([](float Value) { LOG("Slider2 (V): {:.2f}", Value); });
		AutoP->AddChild(Slider2, ChildAlignment::Left);

		// Add spacer before progress bars
		auto Spacer3 = Subsystem.GUI->CreateSpacer(10.f, 15.f);
		Spacer3->SetRenderDepth(175.f);
		AutoP->AddChild(Spacer3, ChildAlignment::Left);

		// Test ProgressBars
		// Horizontal rectangle progress bar (LTR)
		auto Bar1 = Subsystem.GUI->CreateProgressBar({ 180.f, 20.f }, 0.5f);
		Bar1->SetRenderDepth(75.f);
		Bar1->SetBackgroundColor(color{ 50, 50, 50 });
		Bar1->SetFillColor(color{ 100, 200, 100 });
		AutoP->AddChild(Bar1, ChildAlignment::Left);

		// Horizontal progress bar (RTL)
		auto Bar2 = Subsystem.GUI->CreateProgressBar({ 180.f, 20.f }, 0.3f);
		Bar2->SetRenderDepth(75.f);
		Bar2->SetDirection(ProgressDirection::RightToLeft);
		Bar2->SetBackgroundColor(color{ 50, 50, 50 });
		Bar2->SetFillColor(color{ 200, 100, 100 });
		AutoP->AddChild(Bar2, ChildAlignment::Left);

		// Vertical progress bar (bottom to top)
		auto Bar3 = Subsystem.GUI->CreateProgressBar({ 20.f, 100.f }, 0.7f);
		Bar3->SetRenderDepth(75.f);
		Bar3->SetDirection(ProgressDirection::BottomToTop);
		Bar3->SetBackgroundColor(color{ 50, 50, 50 });
		Bar3->SetFillColor(color{ 100, 100, 200 });
		AutoP->AddChild(Bar3, ChildAlignment::Left);
	}

	void MainMenuUI::OnColorButtonClicked()
	{
		LOG("Color button clicked");
		OnPlayButtonClicked.Broadcast();
	}

	void MainMenuUI::OnTextureButtonClicked()
	{
		LOG("Texture button clicked");
		OnPlayButtonClicked.Broadcast();
	}
}

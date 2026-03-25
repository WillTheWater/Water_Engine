// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/SettingsUI.h"
#include "Subsystem/GuiSubsystem.h"

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

	void SettingsUI::Initialize()
	{
		if (bInitialized)
			return;

		SetupLayout();
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
		auto MuteCheckbox = CreateCheckbox("");
		MuteCheckbox->setPosition("85%", 2);
		MuteCheckbox->setSize(25, 25);
		MuteRow->add(MuteCheckbox);
		Layout->add(MuteRow);

		// Channel sliders
		auto AddChannelSlider = [&](const std::string& Name)
		{
			auto Row = tgui::HorizontalLayout::create();
			Row->setSize({ "100%", "30" });
			auto Label = tgui::Label::create(Name);
			Label->setTextSize(16);
			Label->getRenderer()->setTextColor(tgui::Color::White);
			Label->setSize({ "80", "100%" });
			Row->add(Label);
			auto Slider = CreateSlider();
			Row->add(Slider);
			Layout->add(Row);
		};

		AddChannelSlider("Master");
		AddChannelSlider("Music");
		AddChannelSlider("Ambient");
		AddChannelSlider("SFX");
		AddChannelSlider("Voice");
		AddChannelSlider("UI");

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
		auto FullscreenCheckbox = CreateCheckbox("");
		FullscreenCheckbox->setPosition("85%", 2);
		FullscreenCheckbox->setSize(25, 25);
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

	void SettingsUI::OnBackPressed()
	{
		OnBackClicked.Broadcast();
	}

	void SettingsUI::Show()
	{
		if (!bInitialized)
			Initialize();

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

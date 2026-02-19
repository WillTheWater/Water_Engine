// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/PauseUI.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/Panel.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	PauseUI::PauseUI(EngineSubsystem& InSubsystem)
		: Subsystem{ InSubsystem }
	{
		CreateUI();
		Hide();
	}

	PauseUI::~PauseUI() = default;

	void PauseUI::CreateUI()
	{
		// Create pause menu panel centered on screen
		MenuPanel = Subsystem.GUI->CreatePanel({ 400.f, 400.f }, color{ 30, 30, 30, 240 }, color{ 100, 100, 100 }, 2.f);
		MenuPanel->SetAnchorPosition(Anchor::Center, Anchor::Center);
		MenuPanel->SetRenderDepth(300.f);

		// Create Continue button
		ContinueButton = Subsystem.GUI->CreateButton("Continue", vec2f{ 200.f, 50.f });
		ContinueButton->SetRenderDepth(350.f);
		ContinueButton->SetHoverSound(GC.DefaultButtonHoverSound);
		ContinueButton->SetClickSound(GC.DefaultButtonClickSound);
		ContinueButton->OnClicked.Bind(this, &PauseUI::OnContinueButtonClicked);
		MenuPanel->AddChild(ContinueButton, Anchor::TopCenter, Anchor::TopCenter, { 0.f, 40.f });

		// Create Settings button
		SettingsButton = Subsystem.GUI->CreateButton("Settings", vec2f{ 200.f, 50.f });
		SettingsButton->SetRenderDepth(350.f);
		SettingsButton->SetHoverSound(GC.DefaultButtonHoverSound);
		SettingsButton->SetClickSound(GC.DefaultButtonClickSound);
		SettingsButton->OnClicked.Bind(this, &PauseUI::OnSettingsButtonClicked);
		MenuPanel->AddChild(SettingsButton, Anchor::TopCenter, Anchor::TopCenter, { 0.f, 110.f });

		// Create Quit button
		QuitButton = Subsystem.GUI->CreateButton("Quit", vec2f{ 200.f, 50.f });
		QuitButton->SetRenderDepth(350.f);
		QuitButton->SetHoverSound(GC.DefaultButtonHoverSound);
		QuitButton->SetClickSound(GC.DefaultButtonClickSound);
		QuitButton->OnClicked.Bind(this, &PauseUI::OnQuitButtonClicked);
		MenuPanel->AddChild(QuitButton, Anchor::TopCenter, Anchor::TopCenter, { 0.f, 180.f });
	}

	void PauseUI::Show()
	{
		if (MenuPanel) MenuPanel->SetVisible(true);
	}

	void PauseUI::Hide()
	{
		if (MenuPanel) MenuPanel->SetVisible(false);
	}

	bool PauseUI::IsVisible() const
	{
		return MenuPanel && MenuPanel->IsVisible();
	}

	void PauseUI::Toggle()
	{
		if (IsVisible())
		{
			Hide();
		}
		else
		{
			Show();
		}
	}

	void PauseUI::OnContinueButtonClicked()
	{
		LOG("Continue button clicked");
		OnContinueClicked.Broadcast();
		Hide();
	}

	void PauseUI::OnSettingsButtonClicked()
	{
		LOG("Settings button clicked from pause menu");
		OnSettingsClicked.Broadcast();
	}

	void PauseUI::OnQuitButtonClicked()
	{
		LOG("Quit button clicked from pause menu");
		OnQuitClicked.Broadcast();
	}
}

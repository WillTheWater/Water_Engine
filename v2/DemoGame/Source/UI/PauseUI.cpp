// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/PauseUI.h"
#include "UI/Widget/Panel.h"
#include "UI/Widget/Button.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"
#include "UI/UILayerOrder.h"

namespace we
{
    PauseUI::PauseUI(EngineSubsystem& Subsystem)
        : Subsystem{ Subsystem }
    {
        CreateUI();
        Hide();
    }

    PauseUI::~PauseUI()
    {
    }

    void PauseUI::CreateUI()
    {
        MenuPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem, EC.DefaultPanel);
        MenuPanel->SetZOrder(LAYER_PAUSE_MENU);
        MenuPanel->SetAnchorPosition(Anchor::Center, Anchor::Center);

        ResumeButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Resume", EC.DefaultButton);
        ResumeButton->SetLocalOffset({ 0, -40 });
        ResumeButton->OnClicked.Bind(this, &PauseUI::OnResumeClicked);

        QuitButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", EC.DefaultButton);
        QuitButton->SetLocalOffset({ 0, 60 });
        QuitButton->OnClicked.Bind(this, &PauseUI::OnQuitClicked);

        MenuPanel->AddChild(ResumeButton, Anchor::Center, Anchor::Center, {0,-10});
        MenuPanel->AddChild(QuitButton, Anchor::Center, Anchor::Center, { 0,10 });
    }

    void PauseUI::Show()
    {
        if (MenuPanel)
        {
            MenuPanel->SetVisible(true);
            bVisible = true;
        }
    }

    void PauseUI::Hide()
    {
        if (MenuPanel)
        {
            MenuPanel->SetVisible(false);
            bVisible = false;
        }
    }

    void PauseUI::OnResumeClicked()
    {
        OnResume.Broadcast();
    }

    void PauseUI::OnQuitClicked()
    {
        Subsystem.GameState->OnQuitRequested.Broadcast();
    }
}
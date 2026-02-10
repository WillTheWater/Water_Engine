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
        if (MenuPanel)
        {
            Subsystem.GUI->DestroyWidget(MenuPanel.get());
        }
    }

    void PauseUI::CreateUI()
    {
        MenuPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem, EC.DefaultPanel);
        MenuPanel->SetLocalOffset({ EC.WindowSize.x * 0.5f, EC.WindowSize.y * 0.5f });
        MenuPanel->SetLocalScale({ 1.5f, 1.5f });
        MenuPanel->SetZOrder(LAYER_PAUSE_MENU);

        ResumeButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Resume", EC.DefaultButton);
        ResumeButton->SetLocalOffset({ 0, -40 });
        ResumeButton->OnClicked.Bind(this, &PauseUI::OnResumeClicked);

        QuitButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", EC.DefaultButton);
        QuitButton->SetLocalOffset({ 0, 60 });
        QuitButton->OnClicked.Bind(this, &PauseUI::OnQuitClicked);

        MenuPanel->AddChild(ResumeButton);
        MenuPanel->AddChild(QuitButton);
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
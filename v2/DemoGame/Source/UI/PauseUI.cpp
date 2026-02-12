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
        //MenuPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem, EC.DefaultPanel);
        //MenuPanel->SetLocalOffset({ EC.WindowSize.x * 0.5f, EC.WindowSize.y * 0.5f });
        //MenuPanel->SetLocalScale({ 1.5f, 1.5f });
        //MenuPanel->SetZOrder(LAYER_PAUSE_MENU);

        //ResumeButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Resume", EC.DefaultButton);
        //ResumeButton->SetLocalOffset({ 0, -40 });
        //ResumeButton->OnClicked.Bind(this, &PauseUI::OnResumeClicked);

        //QuitButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", EC.DefaultButton);
        //QuitButton->SetLocalOffset({ 0, 60 });
        //QuitButton->OnClicked.Bind(this, &PauseUI::OnQuitClicked);

        ////MenuPanel->AddChild(ResumeButton);
        ////MenuPanel->AddChild(QuitButton);

        //// Anchor test widgets
        //// Top-left corner, 20px padding
        //auto TopLeft = Subsystem.GUI->CreateWidget<Panel>(Subsystem);
        //TopLeft->SetAnchorPosition(Anchor::TopLeft, Anchor::Center, { 20, 20 });

        //// Top-right corner
        //auto TopRight = Subsystem.GUI->CreateWidget<Panel>(Subsystem);
        //TopRight->SetAnchorPosition(Anchor::TopRight, Anchor::CenterRight, { -20, 20 });

        //// Bottom-left
        //auto BottomLeft = Subsystem.GUI->CreateWidget<Panel>(Subsystem);
        //BottomLeft->SetAnchorPosition(Anchor::BottomLeft, Anchor::BottomLeft, { 20, -20 });

        //// Bottom-right
        //auto BottomRight = Subsystem.GUI->CreateWidget<Panel>(Subsystem, vec2f{200,200});
        //BottomRight->SetAnchorPosition(Anchor::BottomRight, Anchor::BottomRight, {-4, -4});

        //// Center
        //auto Center = Subsystem.GUI->CreateWidget<Panel>(Subsystem, vec2f(300,300));
        //Center->SetAnchorPosition(Anchor::Center, Anchor::Center, { 0, 0 });
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
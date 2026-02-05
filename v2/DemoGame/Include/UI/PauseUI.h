// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Utility/Delegate.h"

namespace we
{
    struct EngineSubsystem;
    class Panel;
    class Button;

    class PauseUI
    {
    public:
        PauseUI(EngineSubsystem& Subsystem);
        ~PauseUI();

        void Show();
        void Hide();
        bool IsVisible() const { return bVisible; }

        Delegate<>OnResume;

    private:
        void CreateUI();
        void OnResumeClicked();
        void OnQuitClicked();

        EngineSubsystem& Subsystem;

        shared<Panel> MenuPanel;
        shared<Button> ResumeButton;
        shared<Button> QuitButton;

        bool bVisible = false;
    };
}
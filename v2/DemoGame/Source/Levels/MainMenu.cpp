// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Levels/LevelOne.h"
#include "Utility/Log.h"

namespace we
{
    MainMenu::MainMenu(EngineSubsystem& Subsystem)
        : World(Subsystem)
    {
        BgTexture = Asset().LoadTexture(EC.DefaultBackground);
        Background.emplace(*BgTexture);
        CreateUI();
    }

    MainMenu::~MainMenu()
    {
    }

    void MainMenu::Render()
    {
        if (Background) { Subsystem.Render->Draw(*Background); }

        RenderActors();
    }

    void MainMenu::CreateUI()
    {
    }

    void MainMenu::HandlePlay()
    {
        LOG("Play clicked!");
        OnPlayPressed.Broadcast();
    }

    void MainMenu::HandleQuit()
    {
        LOG("Quit clicked!");
        OnQuitPressed.Broadcast();
    }
}
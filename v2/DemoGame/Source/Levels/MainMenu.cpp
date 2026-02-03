// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
    MainMenu::MainMenu(EngineSubsystem& Subsystem)
        : World(Subsystem)
    {
        BgTexture = Asset().LoadTexture(EC.DefaultBackground);
        Background.emplace(*BgTexture);
    }

    void MainMenu::Render()
    {
        if (Background) { Subsystem.Render->Draw(*Background); }

        RenderActors();
    }
}
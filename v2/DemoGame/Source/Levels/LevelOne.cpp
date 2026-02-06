// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
    LevelOne::LevelOne(EngineSubsystem& Subsystem)
        : World(Subsystem)
    {
        BgTexture = Asset().LoadTexture(EC.LevelOneBackground);
        Background.emplace(*BgTexture);
    }

    void LevelOne::Render()
    {
        if (Background) { Subsystem.Render->Draw(*Background); }

        RenderActors();
    }
}
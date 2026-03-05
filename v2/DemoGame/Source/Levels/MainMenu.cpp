// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Utility/Log.h"

namespace we
{
    MainMenu::MainMenu(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    void MainMenu::BeginPlay()
    {
        LOG("MainMenu::BeginPlay")
        auto TestActor = SpawnActor<Actor>();
    }

    void MainMenu::Tick(float DeltaTime)
    {
    }
}

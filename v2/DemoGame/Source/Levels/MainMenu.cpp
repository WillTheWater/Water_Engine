// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Subsystem/TimerSubsystem.h"
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
        TestActor = SpawnActor<Actor>().lock();
        
        DestroyTimerHandle = GetTimer().SetTimer(GetObject(), &MainMenu::DestroyTestActor, 1.0f, false);
    }

    void MainMenu::Tick(float DeltaTime)
    {
    }

    void MainMenu::DestroyTestActor()
    {
        LOG("Destroying TestActor!")
        if (TestActor)
        {
            TestActor->Destroy();
            TestActor = nullptr;
        }
    }
}

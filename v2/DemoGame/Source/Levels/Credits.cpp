// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/Credits.h"
#include "Framework/World/Actor.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    Credits::Credits(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    void Credits::BeginPlay()
    {
    }

    void Credits::Tick(float DeltaTime)
    {
    }

    void Credits::EndPlay()
    {
    }

    void Credits::Quit()
    {
        Subsystem.Quit();
    }
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WorldSubsystem.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
    WorldSubsystem::WorldSubsystem(EngineSubsystem& Subsystem)
        : Subsystem{ Subsystem }
    {
    }

    void WorldSubsystem::UnloadWorld()
    {
        CurrentWorld = nullptr;
    }
}
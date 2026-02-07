// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WorldSubsystem.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    WorldSubsystem::WorldSubsystem(EngineSubsystem& Subsystem)
        : Subsystem{ Subsystem }
    {
    }

    void WorldSubsystem::RegisterWorld(size_t StateID, WorldFactory Factory)
    {
        WorldRegistry[StateID] = std::move(Factory);
    }

    void WorldSubsystem::LoadWorldForState(const IGameStateToken* State)
    {
        if (!State)
        {
            ERROR("Cannot load world for null state");
            return;
        }

        size_t StateID = State->GetStateID();
        auto It = WorldRegistry.find(StateID);

        if (It == WorldRegistry.end())
        {
            ERROR("No world registered for state ID: {} ({})", StateID, State->GetDebugName());
            return;
        }

        UnloadWorld();
        CurrentWorld = It->second(Subsystem);
    }

    void WorldSubsystem::UnloadWorld()
    {
        CurrentWorld = nullptr;
    }
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WorldSubsystem.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    WorldSubsystem::WorldSubsystem(EngineSubsystem& InSub)
        : Subsystem(InSub)
    {
        LOG("WorldSubsystem initialized");
    }

    WorldSubsystem::~WorldSubsystem() = default;

    void WorldSubsystem::LoadWorld(const string& Name)
    {
        UnloadWorld();

        CurrentWorld = Factory.Create(Name, Subsystem);
        if (CurrentWorld)
        {
            CurrentWorld->PreConstruct();
            LOG("Loaded world: {}", Name);
        }
        else
        {
            ERROR("Failed to load world '{}': not registered", Name);
        }
    }

    void WorldSubsystem::StartPlay()
    {
        if (CurrentWorld)
        {
            CurrentWorld->BeginPlayGlobal();
        }
    }

    void WorldSubsystem::EndPlay()
    {
        if (CurrentWorld)
        {
            CurrentWorld->EndPlayGlobal();
        }
    }

    void WorldSubsystem::UnloadWorld()
    {
        if (CurrentWorld)
        {
            CurrentWorld = nullptr;
            LOG("Unloaded world");
        }
    }

    void WorldSubsystem::Tick(float DeltaTime)
    {
        if (CurrentWorld)
        {
            CurrentWorld->TickGlobal(DeltaTime);
        }
    }

    void WorldSubsystem::Render()
    {
        if (CurrentWorld)
        {
            CurrentWorld->Render();
        }
    }
}

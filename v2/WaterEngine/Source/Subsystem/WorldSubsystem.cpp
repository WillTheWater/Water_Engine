// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WorldSubsystem.h"
#include "Framework/World/World.h"

namespace we
{
	void WorldSubsystem::Tick(float DeltaTime)
	{
        if (PendingWorld)
        {
            if (CurrentWorld)
            {
                CurrentWorld->EndPlay();
            }

            CurrentWorld = PendingWorld;
            PendingWorld = nullptr;
            CurrentWorld->StartPlay();
        }

        if (CurrentWorld)
        {
            CurrentWorld->StartTick(DeltaTime);
            GarbageCollect(DeltaTime);
        }
	}

    void WorldSubsystem::GarbageCollect(float DeltaTime)
    {
        GCTimer += DeltaTime;
        if (GCTimer >= GCInterval)
        {
            GCTimer = 0.0f;
            CurrentWorld->GarbageCollection();
        }
    }
}
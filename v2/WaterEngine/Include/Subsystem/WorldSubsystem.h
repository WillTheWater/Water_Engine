// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    class World;

    class WorldSubsystem
    {
    public:
        void BeginPlay();
        void Tick(float DeltaTime);

        shared<World> GetCurrentWorld() const { return CurrentWorld; }
        bool HasPendingWorld() const { return PendingWorld != nullptr; }
        
        template<typename WorldType>
        void CreateWorld();

    private:
        shared<World> CurrentWorld;
        shared<World> PendingWorld;
    };

    template<typename WorldType>
    inline void WorldSubsystem::CreateWorld()
    {
        PendingWorld = make_shared<WorldType>(*this);
    }
}
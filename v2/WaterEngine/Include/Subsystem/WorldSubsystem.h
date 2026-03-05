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
        void Tick(float DeltaTime);

        shared<World> GetCurrentWorld() const { return CurrentWorld; }
        bool HasPendingWorld() const { return PendingWorld != nullptr; }

        // Returns ordered drawables from current world
        vector<const drawable*>& GetOrderedDrawables() const;
        
        template<typename WorldType>
        void CreateWorld();

    private:
        void GarbageCollect(float DeltaTime);

    private:
        shared<World> CurrentWorld;
        shared<World> PendingWorld;

        float GCTimer = 0.0f;
        static constexpr float GCInterval = 3.0f;
    };

    template<typename WorldType>
    inline void WorldSubsystem::CreateWorld()
    {
        PendingWorld = make_shared<WorldType>(*this);
    }
}

// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    class World;
    class PhysicsSubsystem;

    class WorldSubsystem
    {
    public:
        void Tick(float DeltaTime);

        void SetPhysicsRef(shared<PhysicsSubsystem> InPhysics);
        PhysicsSubsystem& GetPhysics();

        shared<World> GetCurrentWorld() const { return CurrentWorld; }
        bool HasPendingWorld() const { return PendingWorld != nullptr; }
        vector<const drawable*>& GetOrderedDrawables() const;
        
        template<typename WorldType>
        void CreateWorld();
        
        template<typename WorldType>
        void LoadWorld() { CreateWorld<WorldType>(); }

        void Pause() { bIsPaused = true; }
        void Resume() { bIsPaused = false; }
        void TogglePause() { bIsPaused = !bIsPaused; }
        bool IsPaused() const { return bIsPaused; }
        void Quit() { bShouldQuit = true; }
        bool ShouldQuit() const { return bShouldQuit; }

    private:
        void GarbageCollect(float DeltaTime);

    private:
        weak<PhysicsSubsystem> Physics;
        
        shared<World> CurrentWorld;
        shared<World> PendingWorld;

        float GCTimer = 0.0f;
        static constexpr float GCInterval = 3.0f;
        
        bool bIsPaused = false;
        bool bShouldQuit = false;
    };

    template<typename WorldType>
    inline void WorldSubsystem::CreateWorld()
    {
        PendingWorld = make_shared<WorldType>(*this);
    }
}

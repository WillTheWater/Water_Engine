// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"
#include "Framework/World/WorldFactory.h"

namespace we
{
    struct EngineSubsystem;

    // =========================================================================
    // WorldSubsystem - Manages world lifecycle and current world
    // =========================================================================
    class WorldSubsystem
    {
    public:
        WorldSubsystem(EngineSubsystem& Sub);
        ~WorldSubsystem();

        // Access factory for registration
        WorldFactory& GetFactory() { return Factory; }

        // World management - template factory (calls PreConstruct only)
        template<typename WorldType>
        void LoadWorld()
        {
            static_assert(std::is_base_of_v<World, WorldType>, "WorldType must derive from World");
            CurrentWorld = Factory.Create<WorldType>(Subsystem);
            if (CurrentWorld)
            {
                CurrentWorld->PreConstruct();
            }
        }

        // Load world by name (uses registered factory)
        void LoadWorld(const string& Name);

        // Start Play mode - calls BeginPlay on current world
        void StartPlay();
        
        // End Play mode - calls EndPlay on current world
        void EndPlay();

        // Stop Play mode - unloads current world
        void UnloadWorld();

        // Get current world
        World* GetCurrentWorld() const { return CurrentWorld.get(); }
        bool HasActiveWorld() const { return CurrentWorld != nullptr; }

        // Delegates to current world
        void Tick(float DeltaTime);
        void Render();

    private:
        WorldFactory Factory;  // Factory is part of world management, not a core subsystem
        EngineSubsystem& Subsystem;
        unique<World> CurrentWorld;
    };
}

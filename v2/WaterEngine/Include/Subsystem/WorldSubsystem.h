// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
    class World;
    struct EngineSubsystem;

    class WorldSubsystem
    {
    public:
        explicit WorldSubsystem(EngineSubsystem& Subsystem);
        template<typename WorldType>
        weak<WorldType> LoadWorld();

        void UnloadWorld();
        shared<World> GetCurrentWorld() const { return CurrentWorld; }
        bool HasWorld() const { return CurrentWorld != nullptr; }

    private:
        EngineSubsystem& Subsystem;
        shared<World> CurrentWorld;
    };

    template<typename WorldType>
    inline weak<WorldType> WorldSubsystem::LoadWorld()
    {
        shared<WorldType> NewWorld = make_shared<WorldType>(Subsystem);
        CurrentWorld = NewWorld;
        return NewWorld;
    }
}
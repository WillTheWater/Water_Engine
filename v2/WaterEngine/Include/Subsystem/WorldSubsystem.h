// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    // ========================= REGISTRY MACRO =========================
#define RegisterLevel(WORLD) \
    Subsystem.World->RegisterWorld( \
        MakeState(EGameState::WORLD)->GetStateID(), \
        [](EngineSubsystem& Sub) { return make_shared<WORLD>(Sub); } \
    )

    class World;
    struct EngineSubsystem;
    class IGameStateToken;

    using WorldFactory = std::function<shared<World>(EngineSubsystem&)>;

    class WorldSubsystem
    {
    public:
        explicit WorldSubsystem(EngineSubsystem& Subsystem);
        template<typename WorldType>
        weak<WorldType> LoadWorld();

        void RegisterWorld(size_t StateID, WorldFactory Factory);

        // Load world for given state (looks up factory, creates, sets current)
        void LoadWorldForState(const IGameStateToken* State);

        void UnloadWorld();
        shared<World> GetCurrentWorld() const { return CurrentWorld; }
        bool HasWorld() const { return CurrentWorld != nullptr; }

    private:
        EngineSubsystem& Subsystem;
        shared<World> CurrentWorld;
        dictionary<size_t, WorldFactory> WorldRegistry;
    };

    template<typename WorldType>
    inline weak<WorldType> WorldSubsystem::LoadWorld()
    {
        shared<WorldType> NewWorld = make_shared<WorldType>(Subsystem);
        CurrentWorld = NewWorld;
        return NewWorld;
    }
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/IGameStateToken.h"
#include <magic_enum/magic_enum.hpp>

namespace we
{
    

    enum class EGameState : uint8
    {
        None,
        MainMenu,
        LevelOne
    };

    class GameStateToken : public IGameStateToken
    {
    public:
        explicit GameStateToken(EGameState State, string Qualifier = "")
            : StateType(State), Qualifier(std::move(Qualifier))
        {
        }

        string GetDebugName() const override
        {
            auto name = magic_enum::enum_name(StateType);
            if (Qualifier.empty())
                return string(name);
            return std::format("{}:{}", name, Qualifier);
        }

        size_t GetStateID() const override
        {
            size_t baseID = static_cast<size_t>(StateType);
            if (Qualifier.empty()) return baseID;

            return baseID ^ (std::hash<string>{}(Qualifier) << 1);
        }


    public:
        EGameState GetState() const { return StateType; }
        const string& GetQualifier() const { return Qualifier; }
        bool Is(EGameState State) const { return StateType == State; }

        bool IsMenu() const { return StateType == EGameState::MainMenu; }
        bool IsLevelOne() const { return StateType == EGameState::LevelOne; }
        bool IsGameplay() const { return IsLevelOne(); }
        bool HasWorld() const { return IsMenu() || IsLevelOne(); }

    private:
        EGameState StateType;
        string Qualifier;
    };

    inline shared<GameStateToken> MakeState(EGameState State, string Qualifier = "")
    {
        return make_shared<GameStateToken>(State, std::move(Qualifier));
    }
}
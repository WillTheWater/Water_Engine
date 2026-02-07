// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Character/PlayerCharacter.h"
#include "Utility/Log.h"

namespace we
{
    LevelOne::LevelOne(EngineSubsystem& Subsystem)
        : World(Subsystem)
    {
        BgTexture = Asset().LoadTexture(EC.LevelOneBackground);
        Background.emplace(*BgTexture);
    }

    void LevelOne::BeginPlay()
    {
        LOG("Level One Started - Timer set for 3 seconds");
        TestTimer = TimerManager::Get().SetTimer(weak_from_this(), &LevelOne::OnTimerFinished, 3.0f, false);

        auto PlayerRef = SpawnActor<Player>(EC.CharacterSheet);
        if (auto P = PlayerRef.lock())
        {
            P->SetPosition(EC.WindowSize / 2.f);
            P->SetScale({ 4,4 });
        }

        World::BeginPlay();
        Subsystem.Cursor->SetVisibility(false);
    }

    void LevelOne::Tick(float DeltaTime)
    {
    }

    void LevelOne::Render()
    {
        if (Background) { Subsystem.Render->Draw(*Background); }

        RenderActors();
    }

    void LevelOne::OnTimerFinished()
    {
        LOG("Timer Worked!");
        TestTimer.Invalidate();
    }
}
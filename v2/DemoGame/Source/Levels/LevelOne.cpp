// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Framework/World/Actor.h"
#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"

namespace we
{
    LevelOne::LevelOne(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    void LevelOne::BeginPlay()
    {
        BG = LoadAsset().LoadTexture("Assets/Textures/Game/level1.png");
        BGImage = SpawnActor<Actor>().lock();
        BGImage->SetSprite(BG);
    }

    void LevelOne::Tick(float DeltaTime)
    {

    }
}

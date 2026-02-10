// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Character/PlayerCharacter.h"
#include "Utility/Log.h"

// ========== TEST ===========
#include "Interface/Component/PhysicsComponent.h"
#include "Subsystem/PhysicsSubsystem.h"

namespace we
{
    // Test actors
    class CollisionTestActor : public Actor
    {
    public:
        CollisionTestActor(World* OwningWorld, const vec2f& Position, float Radius)
            : Actor(OwningWorld)
        {
            SetPosition(Position);
            auto Phys = CreateComponent<PhysicsComponent>(this);
            Phys->SetCircleShape(Radius);
            Phys->SetDebugDraw(true);
            Phys->SetBodyType(BodyType::Static);
            Phys->SetSensor(true);
            Physics().RegisterComponent(Phys.get());
        }
    };

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
        }

        SpawnActor<CollisionTestActor>(vec2f{ 200, 200 }, 50.0f);
        SpawnActor<CollisionTestActor>(vec2f{ 600, 400 }, 75.0f);
        SpawnActor<CollisionTestActor>(vec2f{ 400, 600 }, 100.0f);

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
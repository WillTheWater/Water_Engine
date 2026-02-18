// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Character/PlayerCharacter.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Utility/Log.h"
#include "GameConfig.h"
#include "EngineConfig.h"

namespace we
{
    // Test actor with physics that ticks manually
    class CollisionTestActor : public Actor
    {
    public:
        CollisionTestActor(World* OwningWorld, const vec2f& Position, float Radius)
            : Actor(OwningWorld, EC.WindowIcon)
        {
            SetPosition(Position);
        }

        virtual void BeginPlay() override
        {
            // Create physics component for collision detection
            PhysComp = make_shared<PhysicsComponent>(this);
            PhysComp->SetCircleShape(50.0f);
            PhysComp->SetBodyType(BodyType::Static);
            PhysComp->BeginPlay();
            
            Actor::BeginPlay();
        }

        virtual void Tick(float DeltaTime) override
        {
            // Tick physics component manually - THIS IS THE KEY FIX FOR DEBUG DRAW
            if (PhysComp)
            {
                PhysComp->Tick(DeltaTime);
            }
            
            Actor::Tick(DeltaTime);
        }

        virtual void Destroy() override
        {
            if (PhysComp)
            {
                PhysComp->EndPlay();
            }
            Actor::Destroy();
        }

    protected:
        shared<PhysicsComponent> PhysComp;
    };

    LevelOne::LevelOne(EngineSubsystem& Subsystem)
        : World(Subsystem)
    {
        BgTexture = LoadAsset().LoadTextureSync(GC.LevelOneBackground);
        if (BgTexture)
        {
            Background.emplace(*BgTexture);
            auto bounds = Background->getLocalBounds();
            Background->setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
            Background->setPosition(vec2f(EC.RenderResolution) / 2.0f);
            AddRenderDepth(&*Background, -1000.0f);
        }
    }

    void LevelOne::BeginPlay()
    {
        // Spawn player in center
        auto PlayerRef = SpawnActor<Player>();
        if (auto P = PlayerRef.lock())
        {
            P->SetPosition(EC.RenderResolution / 2.f);
        }

        // Spawn test collision actors
        SpawnActor<CollisionTestActor>(vec2f{ 200, 200 }, 50.0f);
        SpawnActor<CollisionTestActor>(vec2f{ 600, 400 }, 75.0f);
        SpawnActor<CollisionTestActor>(vec2f{ 400, 600 }, 100.0f);

        World::BeginPlay();
        
        Subsystem.Cursor->SetVisibility(false);
        LOG("LevelOne: Started");
    }

    void LevelOne::Tick(float DeltaTime)
    {
        World::Tick(DeltaTime);
    }
}

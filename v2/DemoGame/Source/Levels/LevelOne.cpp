// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Character/PlayerCharacter.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Utility/Log.h"
#include "GameConfig.h"
#include "EngineConfig.h"

namespace we
{
    // =========================================================================
    // Trigger Test Actor - Detects overlaps but doesn't block movement
    // Uses Box2D sensor fixture
    // =========================================================================
    class TriggerTestActor : public Actor
    {
    public:
        TriggerTestActor(World* OwningWorld, const vec2f& Position, float InRadius)
            : Actor(OwningWorld, EC.WindowIcon)
            , Radius{ InRadius }
        {
            SetPosition(Position);
            SetScale({ 0.5f, 0.5f });
        }

        virtual void BeginPlay() override
        {
            PhysicsComp = make_shared<PhysicsComponent>(this);
            PhysicsComp->SetBodyType(BodyType::Static);
            PhysicsComp->SetCircleShape(Radius);
            PhysicsComp->SetSensor(true);  // Sensor = no blocking, just overlap detection
            PhysicsComp->BeginPlay();
            Actor::BeginPlay();
        }

        virtual void Tick(float DeltaTime) override
        {
            if (PhysicsComp) PhysicsComp->Tick(DeltaTime);
            Actor::Tick(DeltaTime);
        }

        virtual void Destroy() override
        {
            if (PhysicsComp) PhysicsComp->EndPlay();
            Actor::Destroy();
        }

    protected:
        shared<PhysicsComponent> PhysicsComp;
        float Radius = 60.0f;
    };

    // =========================================================================
    // Blocking Test Actor - Circle that blocks movement
    // Uses Box2D static body
    // =========================================================================
    class BlockingTestActor : public Actor
    {
    public:
        BlockingTestActor(World* OwningWorld, const vec2f& Position, float InRadius)
            : Actor(OwningWorld, EC.WindowIcon)
            , Radius{ InRadius }
        {
            SetPosition(Position);
            SetScale({ 0.8f, 0.8f });
        }

        virtual void BeginPlay() override
        {
            PhysicsComp = make_shared<PhysicsComponent>(this);
            PhysicsComp->SetBodyType(BodyType::Static);
            PhysicsComp->SetCircleShape(Radius);
            PhysicsComp->SetFriction(0.5f);
            PhysicsComp->BeginPlay();
            Actor::BeginPlay();
        }

        virtual void Tick(float DeltaTime) override
        {
            if (PhysicsComp) PhysicsComp->Tick(DeltaTime);
            Actor::Tick(DeltaTime);
        }

        virtual void Destroy() override
        {
            if (PhysicsComp) PhysicsComp->EndPlay();
            Actor::Destroy();
        }

    protected:
        shared<PhysicsComponent> PhysicsComp;
        float Radius = 70.0f;
    };

    // =========================================================================
    // Rectangle Blocking Actor - Box that blocks movement
    // Uses Box2D static body with box shape
    // =========================================================================
    class RectangleBlockingActor : public Actor
    {
    public:
        RectangleBlockingActor(World* OwningWorld, const vec2f& Position)
            : Actor(OwningWorld, EC.WindowIcon)
        {
            SetPosition(Position);
            SetScale({ 0.8f, 0.8f });
        }

        virtual void BeginPlay() override
        {
            PhysicsComp = make_shared<PhysicsComponent>(this);
            PhysicsComp->SetBodyType(BodyType::Static);
            PhysicsComp->SetBoxShape({ 90.0f, 40.0f });
            PhysicsComp->SetFriction(0.5f);
            PhysicsComp->BeginPlay();
            Actor::BeginPlay();
        }

        virtual void Tick(float DeltaTime) override
        {
            if (PhysicsComp) PhysicsComp->Tick(DeltaTime);
            Actor::Tick(DeltaTime);
        }

        virtual void Destroy() override
        {
            if (PhysicsComp) PhysicsComp->EndPlay();
            Actor::Destroy();
        }

    protected:
        shared<PhysicsComponent> PhysicsComp;
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
        auto PlayerRef = SpawnActor<Player>();
        if (auto P = PlayerRef.lock())
        {
            P->SetPosition(EC.RenderResolution / 2.f);
        }

        SpawnActor<TriggerTestActor>(vec2f{ 200, 300 }, 60.0f);
        SpawnActor<BlockingTestActor>(vec2f{ 600, 300 }, 70.0f);
        SpawnActor<RectangleBlockingActor>(vec2f{ 1200, 400 });

        World::BeginPlay();
        Subsystem.Cursor->SetVisibility(false);
        LOG("LevelOne: Started");
    }

    void LevelOne::Tick(float DeltaTime)
    {
        World::Tick(DeltaTime);
    }
}

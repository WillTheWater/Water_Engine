// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Character/PlayerCharacter.h"
#include "Interface/Component/TriggerComponent.h"
#include "Interface/Component/BlockingCollisionComponent.h"
#include "Utility/Log.h"
#include "GameConfig.h"
#include "EngineConfig.h"

namespace we
{
    // =========================================================================
    // Trigger Test Actor - Detects overlaps but doesn't block movement
    // Walk through it to see "TRIGGER BEGIN/END" logs
    // =========================================================================
    class TriggerTestActor : public Actor
    {
    public:
        TriggerTestActor(World* OwningWorld, const vec2f& Position, float Radius)
            : Actor(OwningWorld, EC.WindowIcon)
        {
            SetPosition(Position);
            SetScale({ 0.5f, 0.5f });  // Visual only - smaller icon
        }

        virtual void BeginPlay() override
        {
            // Create trigger component for overlap detection
            TriggerComp = make_shared<TriggerComponent>(this);
            TriggerComp->SetCircleShape(60.0f);  // Trigger radius
            TriggerComp->SetCollisionProfile(ECollisionProfile::Trigger);
            TriggerComp->BeginPlay();
            
            Actor::BeginPlay();
            LOG("TriggerTestActor spawned at {:.0f}, {:.0f} - walk through to trigger!", 
                GetPosition().x, GetPosition().y);
        }

        virtual void Tick(float DeltaTime) override
        {
            if (TriggerComp)
            {
                TriggerComp->Tick(DeltaTime);
            }
            Actor::Tick(DeltaTime);
        }

        virtual void Destroy() override
        {
            if (TriggerComp)
            {
                TriggerComp->EndPlay();
            }
            Actor::Destroy();
        }

    protected:
        shared<TriggerComponent> TriggerComp;
    };

    // =========================================================================
    // Blocking Test Actor - Blocks movement (can't walk through)
    // Try to walk into it to see "BLOCKED" logs
    // =========================================================================
    class BlockingTestActor : public Actor
    {
    public:
        BlockingTestActor(World* OwningWorld, const vec2f& Position, float Radius)
            : Actor(OwningWorld, EC.WindowIcon)
        {
            SetPosition(Position);
            SetScale({ 0.8f, 0.8f });  // Visual indicator
        }

        virtual void BeginPlay() override
        {
            // Create blocking component - prevents movement through it
            BlockComp = make_shared<BlockingCollisionComponent>(this);
            BlockComp->SetCircleShape(70.0f);  // Collision radius
            BlockComp->SetCollisionProfile(ECollisionProfile::WorldStatic);
            BlockComp->BeginPlay();
            
            Actor::BeginPlay();
            LOG("BlockingTestActor spawned at {:.0f}, {:.0f} - try to walk into it!", 
                GetPosition().x, GetPosition().y);
        }

        virtual void Tick(float DeltaTime) override
        {
            if (BlockComp)
            {
                BlockComp->Tick(DeltaTime);
            }
            Actor::Tick(DeltaTime);
        }

        virtual void Destroy() override
        {
            if (BlockComp)
            {
                BlockComp->EndPlay();
            }
            Actor::Destroy();
        }

    protected:
        shared<BlockingCollisionComponent> BlockComp;
    };

    // =========================================================================
    // Capsule Test Actor - Demonstrates capsule collision shape
    // =========================================================================
    class CapsuleTestActor : public Actor
    {
    public:
        CapsuleTestActor(World* OwningWorld, const vec2f& Position)
            : Actor(OwningWorld, EC.WindowIcon)
        {
            SetPosition(Position);
            SetScale({ 0.6f, 0.6f });
        }

        virtual void BeginPlay() override
        {
            // Create a capsule-shaped trigger
            TriggerComp = make_shared<TriggerComponent>(this);
            CapsuleShape Capsule(50.0f, 30.0f);  // HalfHeight=50, Radius=30
            TriggerComp->SetCapsuleShape(Capsule);
            TriggerComp->SetCollisionProfile(ECollisionProfile::Trigger);
            TriggerComp->BeginPlay();
            
            Actor::BeginPlay();
            LOG("CapsuleTestActor spawned at {:.0f}, {:.0f} - capsule shape demo!", 
                GetPosition().x, GetPosition().y);
        }

        virtual void Tick(float DeltaTime) override
        {
            if (TriggerComp)
            {
                TriggerComp->Tick(DeltaTime);
            }
            Actor::Tick(DeltaTime);
        }

        virtual void Destroy() override
        {
            if (TriggerComp)
            {
                TriggerComp->EndPlay();
            }
            Actor::Destroy();
        }

    protected:
        shared<TriggerComponent> TriggerComp;
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
        // Setup level boundaries
        // Box around the screen edges (inset by 50px)
        GetBoundaries().AddBox({ 50, 50 }, { EC.RenderResolution.x - 50, EC.RenderResolution.y - 50 }, 20.0f);
        
        // Angled fence/barrier in the middle
        GetBoundaries().AddSegment({ 300, 400 }, { 500, 600 }, 15.0f);
        
        // Chain of barriers
        GetBoundaries().AddChain({
            { 700, 200 },
            { 800, 300 },
            { 900, 250 },
            { 1000, 350 }
        }, 10.0f);

        // Spawn player in center
        auto PlayerRef = SpawnActor<Player>();
        if (auto P = PlayerRef.lock())
        {
            P->SetPosition(EC.RenderResolution / 2.f);
        }

        // Test 1: Trigger zone (walk through it, see overlap logs)
        SpawnActor<TriggerTestActor>(vec2f{ 200, 300 }, 60.0f);

        // Test 2: Blocking collision (can't walk through, see block logs)
        SpawnActor<BlockingTestActor>(vec2f{ 600, 300 }, 70.0f);

        // Test 3: Capsule shape demo (trigger, shows capsule debug draw)
        SpawnActor<CapsuleTestActor>(vec2f{ 400, 500 });

        World::BeginPlay();
        
        Subsystem.Cursor->SetVisibility(false);
        LOG("LevelOne: Started");
        LOG("Controls: WASD to move");
        LOG("- Walk through cyan circle (left) to trigger overlap");
        LOG("- Walk into orange circle (right) to test blocking");
        LOG("- Walk through cyan capsule (bottom) to see capsule shape");
    }

    void LevelOne::Tick(float DeltaTime)
    {
        World::Tick(DeltaTime);
    }
}

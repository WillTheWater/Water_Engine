// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Level/LevelObject.h"
#include "Framework/World/Actor.h"
#include "Component/PhysicsComponent.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	// Debug Only
	void LevelObjectActor::GetDrawables(vector<const drawable*>& OutDrawables) const
	{
		Actor::GetDrawables(OutDrawables);

		if (PhysicsComp && PhysicsComp->IsDebugDrawEnabled())
		{
			if (const auto* Debug = PhysicsComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}
	}

	LevelObject::LevelObject(
		World& InWorld,
		const string& MainTexturePath,
		const string& ShadowTexturePath,
		vec2f Position,
		vec2f InSpriteOrigin,
		float PhysicsRadius,
		vec2f CollisionOffset
	)
		: WorldRef(InWorld)
		, MainTexPath(MainTexturePath)
		, ShadowTexPath(ShadowTexturePath)
		, SpriteOrigin(InSpriteOrigin)
		, PhysicsRadius{ PhysicsRadius }
		, PhysicsOffset{ CollisionOffset }
	{
		// Spawn both actors immediately
		MainActor = WorldRef.SpawnActor<LevelObjectActor>().lock();
		ShadowActor = WorldRef.SpawnActor<Actor>().lock();

		// Set initial position
		MainActor->SetPosition(Position);
		ShadowActor->SetPosition(Position);
	}

	void LevelObject::Init()
	{
		// Load and store textures as members
		MainTex = LoadAsset().LoadTexture(MainTexPath);
		ShadowTex = LoadAsset().LoadTexture(ShadowTexPath);

		// Setup shadow actor (rendered behind)
		ShadowActor->SetSprite(ShadowTex);
		ShadowActor->SetSpriteOrigin(SpriteOrigin);
		ShadowActor->SetCustomRenderDepth(10000.f);

		// Setup main actor
		MainActor->SetSprite(MainTex);
		MainActor->SetSpriteOrigin(SpriteOrigin);

		// Setup physics on main actor
		PhysicsComp = make_shared<PhysicsComponent>(MainActor.get());
		PhysicsComp->SetBodyType(b2_staticBody);
		PhysicsComp->SetShapeType(PhysicsComponent::EShapeType::Circle);
		PhysicsComp->SetShapeSize({ PhysicsRadius, PhysicsRadius });
		PhysicsComp->SetShapeOffset(PhysicsOffset);
		PhysicsComp->BeginPlay();

		// Debug Only
		PhysicsComp->DrawDebug();
		if (auto* LOActor = dynamic_cast<LevelObjectActor*>(MainActor.get()))
		{
			LOActor->SetPhysicsComp(PhysicsComp.get());
		}
	}
}
// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"

namespace we
{
	class Actor;
	class PhysicsComponent;

	// Custom actor that handles debug drawing for physics
	class LevelObjectActor : public Actor
	{
	public:
		LevelObjectActor(World& OwningWorld) : Actor(OwningWorld) {}
		void SetPhysicsComp(PhysicsComponent* InPhysicsComp) { PhysicsComp = InPhysicsComp; }
		void GetDrawables(vector<const drawable*>& OutDrawables) const override;

	private:
		PhysicsComponent* PhysicsComp = nullptr;
	};

	class LevelObject
	{
	public:
		LevelObject(
			World& InWorld,
			const string& MainTexturePath,
			const string& ShadowTexturePath,
			vec2f Position,
			vec2f SpriteOrigin,
			float PhysicsRadius = 50.f,
			vec2f CollisionOffset = {0.f, 0.f}
		);

		void Init();

	private:
		World& WorldRef;
		shared<Actor> MainActor;
		shared<Actor> ShadowActor;
		shared<PhysicsComponent> PhysicsComp;

		shared<texture> MainTex;
		shared<texture> ShadowTex;

		string MainTexPath;
		string ShadowTexPath;
		vec2f SpriteOrigin;
		float PhysicsRadius;
		vec2f PhysicsOffset;
	};
}
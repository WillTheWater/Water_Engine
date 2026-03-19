// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor.h"
#include "box2d/b2_body.h"
#include "Component/PhysicsComponent.h"

namespace we
{
	class CollisionComponent;

	class CollisionActor : public Actor
	{
	public:
		CollisionActor(World& OwningWorld);
		~CollisionActor();
		
		void SetBodyType(b2BodyType Type);
		void SetShapeType(PhysicsComponent::EShapeType Type);
		void SetPhysicsSize(float Size); 
		void SetCollisionSize(float Size);

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		void GetDrawables(vector<const drawable*>& OutDrawables) const override;

	private:
		shared<CollisionComponent> CollisionComp;
		shared<PhysicsComponent> PhysicsComp;
		
		b2BodyType BodyType = b2_staticBody;  //static (immovable)
		PhysicsComponent::EShapeType PhysShapeType = PhysicsComponent::EShapeType::Circle;
		float PhysicsSize = 64.0f;
		float CollisionSize = 64.0f;
	};
}

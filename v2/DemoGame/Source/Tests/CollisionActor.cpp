// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Tests/CollisionActor.h"
#include "Component/CollisionComponent.h"
#include "Component/PhysicsComponent.h"

namespace we
{
	CollisionActor::CollisionActor(World& OwningWorld)
		: Actor(OwningWorld)
	{
	}

	CollisionActor::~CollisionActor() = default;

	void CollisionActor::SetBodyType(b2BodyType Type)
	{
		BodyType = Type;
	}

	void CollisionActor::SetShapeType(PhysicsComponent::EShapeType Type)
	{
		PhysShapeType = Type;
	}

	void CollisionActor::SetPhysicsSize(float Size)
	{
		PhysicsSize = Size;
	}

	void CollisionActor::SetCollisionSize(float Size)
	{
		CollisionSize = Size;
	}

	void CollisionActor::BeginPlay()
	{
		Actor::BeginPlay();

		PhysicsComp = make_shared<PhysicsComponent>(this);
		PhysicsComp->SetBodyType(BodyType);
		PhysicsComp->SetShapeType(PhysShapeType);
		PhysicsComp->SetShapeSize({PhysicsSize, PhysicsSize});
		PhysicsComp->BeginPlay();

		CollisionComp = make_shared<CollisionComponent>(this);
		CollisionComp->SetRadius(CollisionSize);
		CollisionComp->BeginPlay();
		CollisionComp->DrawDebug();
	}

	void CollisionActor::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);

		if (PhysicsComp)
		{
			PhysicsComp->Tick(DeltaTime);
		}

		if (CollisionComp)
		{
			CollisionComp->Tick(DeltaTime);
		}
	}

	void CollisionActor::EndPlay()
	{
		if (PhysicsComp)
		{
			PhysicsComp->EndPlay();
			PhysicsComp.reset();
		}

		if (CollisionComp)
		{
			CollisionComp->EndPlay();
			CollisionComp.reset();
		}

		Actor::EndPlay();
	}

	void CollisionActor::GetDrawables(vector<const drawable*>& OutDrawables) const
	{
		Actor::GetDrawables(OutDrawables);

		if (PhysicsComp)
		{
			if (const auto* Debug = PhysicsComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}

		if (CollisionComp)
		{
			if (const auto* Debug = CollisionComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}
	}
}

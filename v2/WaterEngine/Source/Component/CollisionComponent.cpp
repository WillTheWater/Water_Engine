// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/CollisionComponent.h"
#include "Framework/World/Actor.h"
#include "Framework/World/World.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Core/EngineConfig.h"
#include "Utility/Log.h"
#include "box2d/b2_body.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"

namespace we
{
	CollisionComponent::CollisionComponent(Actor* InOwner)
		: Owner(InOwner)
	{
	}

	CollisionComponent::~CollisionComponent()
	{
		if (Body)
		{
			Body->GetUserData().pointer = 0;

			if (Owner)
			{
				auto& Physics = Owner->GetWorld().GetPhysics();
				Physics.MarkForDestruction(Body);
			}
		}
	}

	void CollisionComponent::SetRadius(float RadiusPixels)
	{
		Radius = RadiusPixels;
	}

	void CollisionComponent::SetShapeOffset(vec2f Offset)
	{
		ShapeOffset = Offset;
		
		// If body exists, recreate to apply offset
		if (Body)
		{
			DestroyBody();
			CreateBody();
		}
	}

	void CollisionComponent::BeginPlay()
	{
		CreateBody();
	}

	void CollisionComponent::CreateBody()
	{
		if (!Owner)
		{
			ERROR("[CollisionComponent] Cannot create body - no owner");
			return;
		}

		auto& Physics = Owner->GetWorld().GetPhysics();

		b2BodyDef BodyDef;
		BodyDef.type = b2_dynamicBody;
		
		// Apply shape offset to position
		vec2f SpawnPos = Owner->GetPosition() + ShapeOffset;
		BodyDef.position = b2Vec2(
			Physics.PixelsToMeters(SpawnPos.x),
			Physics.PixelsToMeters(SpawnPos.y)
		);
		BodyDef.angle = Owner->GetRotation().asRadians();

		Body = Physics.CreateBody(BodyDef);
		if (!Body)
		{
			ERROR("[CollisionComponent] Failed to create body");
			return;
		}

		Body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

		b2CircleShape CircleShape;
		CircleShape.m_radius = Physics.PixelsToMeters(Radius);
		b2FixtureDef FixtureDef;
		FixtureDef.shape = &CircleShape;
		FixtureDef.isSensor = true;
		FixtureDef.density = 0.0f;
		FixtureDef.friction = 0.0f;
		
		uint16 ChannelBits = static_cast<uint16>(CollisionChannel);
		FixtureDef.filter.categoryBits = ChannelBits;
		FixtureDef.filter.maskBits = ChannelBits;  // Only detect same channel

		Body->CreateFixture(&FixtureDef);

		Physics.RegisterContactListener(Body, Owner->GetID());
	}

	void CollisionComponent::Tick(float DeltaTime)
	{
		if (!Body || !Owner)
		{
			return;
		}

		auto& Physics = Owner->GetWorld().GetPhysics();
		
		// Apply offset when updating body position
		vec2f BodyPos = Owner->GetPosition() + ShapeOffset;
		b2Vec2 Position(
			Physics.PixelsToMeters(BodyPos.x),
			Physics.PixelsToMeters(BodyPos.y)
		);
		float Angle = Owner->GetRotation().asRadians();

		Body->SetTransform(Position, Angle);
	}

	void CollisionComponent::EndPlay()
	{
		DestroyBody();
	}

	void CollisionComponent::DestroyBody()
	{
		if (!Body)
		{
			return;
		}

		Body->GetUserData().pointer = 0;

		if (Owner)
		{
			auto& Physics = Owner->GetWorld().GetPhysics();
			Physics.MarkForDestruction(Body);
		}

		Body = nullptr;
		OverlappingIDs.clear();
	}

	Actor* CollisionComponent::GetOwner() const
	{
		return Owner;
	}

	void CollisionComponent::OnComponentBeginOverlap(ActorID OtherID)
	{
		if (!Owner || OtherID == INVALID_ACTOR_ID || OtherID == Owner->GetID())
			return;

		Actor* OtherActor = Owner->GetWorld().FindActor(OtherID);
		if (!OtherActor || OtherActor->IsPendingDestroy())
			return;

		OverlappingIDs.insert(OtherID);
		OnBeginOverlap.Broadcast(OtherActor);
	}

	void CollisionComponent::OnComponentEndOverlap(ActorID OtherID)
	{
		if (OverlappingIDs.erase(OtherID) == 0)
			return;

		// Broadcast only while the actor still exists; a destroyed actor's exit
		// is silent (there is no valid pointer left to hand out).
		if (Owner)
			if (Actor* OtherActor = Owner->GetWorld().FindActor(OtherID))
				OnEndOverlap.Broadcast(OtherActor);
	}

	bool CollisionComponent::IsOtherActor(Actor* CheckActor) const
	{
		if (!CheckActor)
			return false;

		return OverlappingIDs.find(CheckActor->GetID()) != OverlappingIDs.end();
	}

	vector<Actor*> CollisionComponent::GetOtherActors() const
	{
		vector<Actor*> Result;
		if (!Owner)
			return Result;

		Result.reserve(OverlappingIDs.size());
		for (ActorID ID : OverlappingIDs)
		{
			if (Actor* OtherActor = Owner->GetWorld().FindActor(ID))
				Result.push_back(OtherActor);
		}
		return Result;
	}

	const drawable* CollisionComponent::DrawDebug()
	{
		bDebugDrawEnabled = true;
		
		if (!Body || !Owner)
			return nullptr;

		if (!DebugCircle.has_value())
		{
			DebugCircle = circle(Radius);
			DebugCircle->setOrigin({ Radius, Radius });
			DebugCircle->setFillColor(color::Transparent);
			DebugCircle->setOutlineThickness(2.0f);
		}

		DebugCircle->setPosition(Owner->GetPosition() + ShapeOffset);
		DebugCircle->setOutlineColor(IsOverlapping() ? color::Red : color::Green);

		return &DebugCircle.value();
	}

	void CollisionComponent::SetCollisionChannel(ECollisionChannel Channel)
	{
		CollisionChannel = Channel;
		
		if (!Body)
			return;
		
		uint16 ChannelBits = static_cast<uint16>(Channel);
		for (b2Fixture* Fixture = Body->GetFixtureList(); Fixture; Fixture = Fixture->GetNext())
		{
			b2Filter Filter = Fixture->GetFilterData();
			Filter.categoryBits = ChannelBits;
			Filter.maskBits = ChannelBits;
			Fixture->SetFilterData(Filter);
		}
	}
}
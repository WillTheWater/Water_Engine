// =============================================================================
// Water Engine v2.0.0
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
		BodyDef.position = b2Vec2(
			Physics.PixelsToMeters(Owner->GetPosition().x),
			Physics.PixelsToMeters(Owner->GetPosition().y)
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
		b2Vec2 Position(
			Physics.PixelsToMeters(Owner->GetPosition().x),
			Physics.PixelsToMeters(Owner->GetPosition().y)
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
	}

	Actor* CollisionComponent::GetOwner() const
	{
		return Owner;
	}

	void CollisionComponent::OnComponentBeginOverlap(b2Body* OtherBody)
	{
		Actor* OtherActor = GetActorFromBody(OtherBody);
		if (!OtherActor || OtherActor == Owner)
			return;
		
		if (OtherActor->IsPendingDestroy())
			return;
		
		OverlappingActors.insert(OtherActor);
		OnBeginOverlap.Broadcast(OtherActor);
	}

	void CollisionComponent::OnComponentEndOverlap(b2Body* OtherBody)
	{
		Actor* OtherActor = GetActorFromBody(OtherBody);
		
		if (!OtherActor)
		{
			CleanupDestroyedOverlaps();
			return;
		}
		
		OverlappingActors.erase(OtherActor);
		OnEndOverlap.Broadcast(OtherActor);
	}

	bool CollisionComponent::IsOtherActor(Actor* CheckActor) const
	{
		if (!CheckActor)
			return false;
		
		return OverlappingActors.find(CheckActor) != OverlappingActors.end();
	}

	void CollisionComponent::CleanupDestroyedOverlaps()
	{
		for (auto It = OverlappingActors.begin(); It != OverlappingActors.end();)
		{
			if ((*It)->IsPendingDestroy())
			{
				OnEndOverlap.Broadcast(*It);
				It = OverlappingActors.erase(It);
			}
			else
			{
				++It;
			}
		}
	}

	Actor* CollisionComponent::GetActorFromBody(b2Body* Body) const
	{
		if (!Body || !Owner)
			return nullptr;
		
		auto& Physics = Owner->GetWorld().GetPhysics();
		ActorID ID = Physics.GetBodyActorID(Body);
		
		if (ID == INVALID_ACTOR_ID)
			return nullptr;
		
		return Owner->GetWorld().FindActor(ID);
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

		DebugCircle->setPosition(Owner->GetPosition());
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
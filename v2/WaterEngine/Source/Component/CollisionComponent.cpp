// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/CollisionComponent.h"
#include "Framework/World/Actor.h"
#include "Framework/World/World.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "box2d/b2_body.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"
#include "Utility/Log.h"

namespace we
{
	CollisionComponent::CollisionComponent(Actor* InOwner)
		: Owner(InOwner)
	{
	}

	void CollisionComponent::SetRadius(float RadiusPixels)
	{
		Radius = RadiusPixels;
	}

	void CollisionComponent::BeginPlay()
	{
		LOG("[CollisionComponent] BeginPlay on Actor {}", Owner ? Owner->GetID() : 0);
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
		LOG("[CollisionComponent] Creating body with radius {} pixels", Radius);

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
		LOG("[CollisionComponent] Body created, userData set to component pointer {}", reinterpret_cast<void*>(this));

		b2CircleShape CircleShape;
		CircleShape.m_radius = Physics.PixelsToMeters(Radius);
		LOG("[CollisionComponent] Circle shape radius: {} meters", CircleShape.m_radius);

		b2FixtureDef FixtureDef;
		FixtureDef.shape = &CircleShape;
		FixtureDef.isSensor = true;
		FixtureDef.density = 0.0f;
		FixtureDef.friction = 0.0f;

		Body->CreateFixture(&FixtureDef);
		LOG("[CollisionComponent] Fixture created (sensor)");

		Physics.RegisterContactListener(Body, this);
		LOG("[CollisionComponent] Registered with contact listener");
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
		LOG("[CollisionComponent] EndPlay on Actor {}", Owner ? Owner->GetID() : 0);
		DestroyBody();
	}

	void CollisionComponent::DestroyBody()
	{
		if (!Body)
		{
			return;
		}

		LOG("[CollisionComponent] Destroying body");

		if (Owner)
		{
			auto& Physics = Owner->GetWorld().GetPhysics();
			Physics.UnregisterContactListener(Body);
			Physics.MarkForDestruction(Body);
		}

		Body = nullptr;
	}

	Actor* CollisionComponent::GetOwner() const
	{
		return Owner;
	}

	void CollisionComponent::OnBeginOverlap(b2Body* OtherBody)
	{
		// Get the other actor from the body
		Actor* OtherActor = GetActorFromBody(OtherBody);
		
		// Ignore overlaps with same actor (e.g., our own PhysicsComponent body)
		if (!OtherActor || OtherActor == Owner)
			return;
		
		OverlappingActors.insert(OtherActor);
		LOG("[CollisionComponent] OnBeginOverlap with Actor {} - Count: {}", 
			OtherActor->GetID(), OverlappingActors.size());
	}

	void CollisionComponent::OnEndOverlap(b2Body* OtherBody)
	{
		Actor* OtherActor = GetActorFromBody(OtherBody);
		
		if (!OtherActor)
			return;
		
		OverlappingActors.erase(OtherActor);
		LOG("[CollisionComponent] OnEndOverlap with Actor {} - Count: {}", 
			OtherActor->GetID(), OverlappingActors.size());
	}

	bool CollisionComponent::IsOtherActor(Actor* CheckActor) const
	{
		if (!CheckActor)
			return false;
		
		return OverlappingActors.find(CheckActor) != OverlappingActors.end();
	}

	Actor* CollisionComponent::GetActorFromBody(b2Body* Body) const
	{
		if (!Body || !Body->GetUserData().pointer)
			return nullptr;
		
		auto* Component = reinterpret_cast<IActorComponent*>(Body->GetUserData().pointer);
		if (!Component)
			return nullptr;
		
		return Component->GetOwner();
	}

	const drawable* CollisionComponent::DrawDebug()
	{
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
}
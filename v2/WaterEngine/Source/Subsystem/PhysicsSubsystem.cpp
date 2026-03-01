// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/PhysicsSubsystem.h"
#include "Framework/World/Actor.h"
#include "box2d/b2_body.h"
#include "box2d/b2_contact.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"

namespace we
{
	PhysicsSubsystem::PhysicsSubsystem(const EngineConfig::PhysicsConfig& Config)
		: PhysicsWorld{ b2Vec2{Config.Gravity.x, Config.Gravity.y} }
		, PhysicsScale{ Config.PhysicsScale }
		, VelocityIterations{ Config.VelocityIterations }
		, PositionIterations{ Config.PositionIterations }
		, ContactListener{}
		, PendingRemovalListeners{}
	{
		PhysicsWorld.SetContactListener(&ContactListener);
		PhysicsWorld.SetAllowSleeping(false);
	}

	void PhysicsSubsystem::RemovePendingListeners()
	{
		for (auto Listener : PendingRemovalListeners)
		{
			PhysicsWorld.DestroyBody(Listener);
		}
		PendingRemovalListeners.clear();
	}

	void PhysicsSubsystem::Step(float DeltaTime)
	{
		RemovePendingListeners();
		PhysicsWorld.Step(DeltaTime, VelocityIterations, PositionIterations);
	}

	void PhysicsSubsystem::SetGravity(const vec2f& Gravity)
	{
		PhysicsWorld.SetGravity(b2Vec2{ Gravity.x, Gravity.y });
	}

	b2Body* PhysicsSubsystem::AddListener(Actor* Listener)
	{
		if (Listener->IsPendingDestroy()) { return nullptr; }

		// Map PhysicsType to b2BodyType
		b2BodyType BodyType = b2_dynamicBody;
		switch (Listener->GetPhysicsType())
		{
		case PhysicsType::Static:
			BodyType = b2_staticBody;
			break;
		case PhysicsType::Kinematic:
			BodyType = b2_kinematicBody;
			break;
		case PhysicsType::Dynamic:
			BodyType = b2_dynamicBody;
			break;
		default:
			BodyType = b2_dynamicBody;
			break;
		}

		b2BodyDef BodyDef;
		BodyDef.type = BodyType;
		BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(Listener);
		BodyDef.position.Set(Listener->GetActorPosition().x * GetPhysicsScale(), Listener->GetActorPosition().y * GetPhysicsScale());
		BodyDef.angle = Listener->GetActorRotation().asRadians();
		b2Body* Body = PhysicsWorld.CreateBody(&BodyDef);
		b2PolygonShape PhysicsShape;
		auto Extents = Listener->GetActorExtents();
		PhysicsShape.SetAsBox(Extents.x * GetPhysicsScale(), Extents.y * GetPhysicsScale());
		b2FixtureDef FixtureDef;
		FixtureDef.shape = &PhysicsShape;
		FixtureDef.density = 1.f;
		FixtureDef.friction = 0.3f;
		Body->CreateFixture(&FixtureDef);

		return Body;
	}

	void PhysicsSubsystem::RemoveListener(b2Body* LisenterBody)
	{
		PendingRemovalListeners.insert(LisenterBody);
	}

	void PhysicsSubsystem::UpdateBodyCollision(Actor* Listener)
	{
		if (!Listener || Listener->IsPendingDestroy()) { return; }

		b2Body* Body = Listener->GetPhysicsBody();
		if (!Body) { return; }

		for (b2Fixture* f = Body->GetFixtureList(); f; )
		{
			b2Fixture* next = f->GetNext();
			Body->DestroyFixture(f);
			f = next;
		}

		b2PolygonShape PhysicsShape;
		auto Extents = Listener->GetActorExtents();

		if (Extents.x <= 0.f || Extents.y <= 0.f) return;

		PhysicsShape.SetAsBox(Extents.x * GetPhysicsScale(), Extents.y * GetPhysicsScale());

		b2FixtureDef FixtureDef;
		FixtureDef.shape = &PhysicsShape;
		FixtureDef.density = 1.f;
		FixtureDef.friction = 0.3f;
		FixtureDef.isSensor = true;

		Body->CreateFixture(&FixtureDef);
	}

	void PhysicsContactListener::BeginContact(b2Contact* Contact)
	{
		/*Actor* ActorA = reinterpret_cast<Actor*>(Contact->GetFixtureA()->GetBody()->GetUserData().pointer);
		Actor* ActorB = reinterpret_cast<Actor*>(Contact->GetFixtureB()->GetBody()->GetUserData().pointer);

		if (ActorA && !ActorA->IsPendingDestroy())
		{
			ActorA->OnActorBeginOverlap(ActorB);
		}

		if (ActorB && !ActorB->IsPendingDestroy())
		{
			ActorB->OnActorBeginOverlap(ActorA);
		}*/
	}

	void PhysicsContactListener::EndContact(b2Contact* Contact)
	{
		/*Actor* ActorA = nullptr;
		Actor* ActorB = nullptr;

		if (Contact->GetFixtureA() && Contact->GetFixtureA()->GetBody())
		{
			ActorA = reinterpret_cast<Actor*>(Contact->GetFixtureA()->GetBody()->GetUserData().pointer);
		}
		if (Contact->GetFixtureB() && Contact->GetFixtureB()->GetBody())
		{
			ActorB = reinterpret_cast<Actor*>(Contact->GetFixtureB()->GetBody()->GetUserData().pointer);
		}
		if (ActorA && !ActorA->IsPendingDestroy())
		{
			ActorA->OnActorEndOverlap(ActorB);
		}
		if (ActorB && !ActorB->IsPendingDestroy())
		{
			ActorB->OnActorEndOverlap(ActorA);
		}*/
	}
}
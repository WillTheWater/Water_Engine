#include "Framework/PhysicsSystem.h"
#include "Framework/Actor.h"
#include "box2d/b2_body.h"
#include "box2d/b2_contact.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "Framework/Core.h"
#include "Framework/MathUtility.h"

namespace we
{
	unique<PhysicsSystem> PhysicsSystem::Instance{ nullptr };

	PhysicsSystem& PhysicsSystem::Get()
	{
		if (!Instance)
		{
			Instance = std::move(unique<PhysicsSystem>{new PhysicsSystem});
		}
		return *Instance;
	}

	PhysicsSystem::PhysicsSystem()
		: PhysicsWorld{b2Vec2{0.f, 0.f}}
		, PhysicsScale{0.01f}
		, VelocityIterations{8}
		, PositionIterations{3}
		, ContactListener{}
		, PendingRemovalListeners{}
	{
		PhysicsWorld.SetContactListener(&ContactListener);
		PhysicsWorld.SetAllowSleeping(false);
	}

	void PhysicsSystem::RemovePendingListeners()
	{
		for (auto Listener : PendingRemovalListeners)
		{
			PhysicsWorld.DestroyBody(Listener);
		}
		PendingRemovalListeners.clear();
	}

	void PhysicsSystem::Step(float DeltaTime)
	{
		RemovePendingListeners();
		PhysicsWorld.Step(DeltaTime, VelocityIterations, PositionIterations);
	}

	b2Body* PhysicsSystem::AddListener(Actor* Listener)
	{
		if (Listener->IsPendingDestroy()) { return nullptr; }

		b2BodyDef BodyDef;
		BodyDef.type = b2_dynamicBody;
		BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(Listener);
		BodyDef.position.Set(Listener->GetActorLocation().x * GetPhysicsScale(), Listener->GetActorLocation().y * GetPhysicsScale());
		BodyDef.angle = Listener->GetActorRotation().asRadians();
		b2Body* Body = PhysicsWorld.CreateBody(&BodyDef);
		b2PolygonShape PhysicsShape;
		auto Bounds = Listener->GetSpriteBounds();
		PhysicsShape.SetAsBox(Bounds.size.x/2.f * GetPhysicsScale(), Bounds.size.y / 2.f * GetPhysicsScale());
		b2FixtureDef FixtureDef;
		FixtureDef.shape = &PhysicsShape;
		FixtureDef.density = 1.f;
		FixtureDef.friction = 0.3f;
		FixtureDef.isSensor = true;
		Body->CreateFixture(&FixtureDef);

		return Body;
	}

	void PhysicsSystem::RemoveListener(b2Body* LisenterBody)
	{
		PendingRemovalListeners.insert(LisenterBody);
	}

	void PhysicsSystem::Cleanup()
	{
		Instance = std::move(unique<PhysicsSystem>{new PhysicsSystem});
	}

	void PhysicsContactListener::BeginContact(b2Contact* Contact)
	{
		Actor* ActorA = reinterpret_cast<Actor*>(Contact->GetFixtureA()->GetBody()->GetUserData().pointer);
		Actor* ActorB = reinterpret_cast<Actor*>(Contact->GetFixtureB()->GetBody()->GetUserData().pointer);

		if (ActorA && !ActorA->IsPendingDestroy())
		{
			ActorA->OnActorBeginOverlap(ActorB);
		}

		if (ActorB && !ActorB->IsPendingDestroy())
		{
			ActorA->OnActorBeginOverlap(ActorA);
		}
	}

	void PhysicsContactListener::EndContact(b2Contact* Contact)
	{
		Actor* ActorA = nullptr;
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
		}
	}
}
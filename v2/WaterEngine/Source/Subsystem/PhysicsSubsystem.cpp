// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/PhysicsSubsystem.h"
#include "Core/EngineConfig.h"
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_math.h"
#include "box2d/b2_contact.h"
#include "Utility/Log.h"

namespace we
{
	class ContactListener : public b2ContactListener
	{
	public:
		dictionary<b2Body*, IPhysicsContactListener*>* Listeners = nullptr;

		void BeginContact(b2Contact* Contact) override
		{
			if (!Listeners) return;

			b2Body* BodyA = Contact->GetFixtureA()->GetBody();
			b2Body* BodyB = Contact->GetFixtureB()->GetBody();

			auto ItA = Listeners->find(BodyA);
			if (ItA != Listeners->end() && ItA->second)
			{
				ItA->second->OnComponentBeginOverlap(BodyB);
			}

			auto ItB = Listeners->find(BodyB);
			if (ItB != Listeners->end() && ItB->second)
			{
				ItB->second->OnComponentBeginOverlap(BodyA);
			}
		}

		void EndContact(b2Contact* Contact) override
		{
			if (!Listeners) return;

			b2Body* BodyA = Contact->GetFixtureA()->GetBody();
			b2Body* BodyB = Contact->GetFixtureB()->GetBody();

			auto ItA = Listeners->find(BodyA);
			if (ItA != Listeners->end() && ItA->second)
			{
				ItA->second->OnComponentEndOverlap(BodyB);
			}

			auto ItB = Listeners->find(BodyB);
			if (ItB != Listeners->end() && ItB->second)
			{
				ItB->second->OnComponentEndOverlap(BodyA);
			}
		}
	};

	static ContactListener s_ContactListener;

	PhysicsSubsystem::PhysicsSubsystem()
		: PhysicsWorld{ make_unique<b2World>(b2Vec2{ WEConfig.Physics.Gravity.x, WEConfig.Physics.Gravity.y }) }
		, PhysicsScale{ WEConfig.Physics.PhysicsScale }
		, VelocityIterations{ WEConfig.Physics.VelocityIterations }
		, PositionIterations{ WEConfig.Physics.PositionIterations }
	{
		PhysicsWorld->SetAllowSleeping(false);
		s_ContactListener.Listeners = &ContactListeners;
		PhysicsWorld->SetContactListener(&s_ContactListener);
	}

	PhysicsSubsystem::~PhysicsSubsystem()
	{
		ProcessPendingDestruction();
	}

	void PhysicsSubsystem::Tick(float DeltaTime)
	{
		ProcessPendingDestruction();
		PhysicsWorld->Step(DeltaTime, VelocityIterations, PositionIterations);
	}

	void PhysicsSubsystem::SetGravity(vec2f Gravity)
	{
		PhysicsWorld->SetGravity(b2Vec2{ Gravity.x, Gravity.y });
	}

	vec2f PhysicsSubsystem::GetGravity() const
	{
		b2Vec2 Gravity = PhysicsWorld->GetGravity();
		return vec2f{ Gravity.x, Gravity.y };
	}

	b2Body* PhysicsSubsystem::CreateBody(const b2BodyDef& Def)
	{
		return PhysicsWorld->CreateBody(&Def);
	}

	void PhysicsSubsystem::DestroyBody(b2Body* Body)
	{
		if (Body)
		{
			PhysicsWorld->DestroyBody(Body);
		}
	}

	void PhysicsSubsystem::MarkForDestruction(b2Body* Body)
	{
		if (Body)
		{
			PendingDestruction.insert(Body);
		}
	}

	void PhysicsSubsystem::RegisterContactListener(b2Body* Body, IPhysicsContactListener* Listener)
	{
		if (Body && Listener)
		{
			ContactListeners[Body] = Listener;
		}
	}

	void PhysicsSubsystem::UnregisterContactListener(b2Body* Body)
	{
		if (Body)
		{
			ContactListeners.erase(Body);
		}
	}

	void PhysicsSubsystem::ProcessPendingDestruction()
	{
		for (auto* Body : PendingDestruction)
		{
			ContactListeners.erase(Body);
			PhysicsWorld->DestroyBody(Body);
		}
		PendingDestruction.clear();
	}

	vec2f PhysicsSubsystem::PixelsToMeters(vec2f Pixels) const
	{
		return vec2f{ Pixels.x * PhysicsScale, Pixels.y * PhysicsScale };
	}

	vec2f PhysicsSubsystem::MetersToPixels(vec2f Meters) const
	{
		return vec2f{ Meters.x / PhysicsScale, Meters.y / PhysicsScale };
	}
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/PhysicsSubsystem.h"
#include "Core/EngineConfig.h"
#include "Framework/World/World.h"
#include "Framework/World/Actor.h"
#include "Component/CollisionComponent.h"
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
		dictionary<b2Body*, ActorID>* Listeners = nullptr;
		World** CurrentWorldPtr = nullptr;
		vector<PhysicsSubsystem::ContactEvent>* EventQueue = nullptr;

		void BeginContact(b2Contact* Contact) override
		{
			if (!Listeners || !CurrentWorldPtr || !*CurrentWorldPtr || !EventQueue) return;

			b2Body* BodyA = Contact->GetFixtureA()->GetBody();
			b2Body* BodyB = Contact->GetFixtureB()->GetBody();

			// Only queue if at least one body is registered for callbacks
			bool bHasListenerA = Listeners->find(BodyA) != Listeners->end();
			bool bHasListenerB = Listeners->find(BodyB) != Listeners->end();

			if (bHasListenerA || bHasListenerB)
			{
				EventQueue->push_back({ BodyA, BodyB, true });
			}
		}

		void EndContact(b2Contact* Contact) override
		{
			if (!Listeners || !CurrentWorldPtr || !*CurrentWorldPtr || !EventQueue) return;

			b2Body* BodyA = Contact->GetFixtureA()->GetBody();
			b2Body* BodyB = Contact->GetFixtureB()->GetBody();

			// Only queue if at least one body is registered for callbacks
			bool bHasListenerA = Listeners->find(BodyA) != Listeners->end();
			bool bHasListenerB = Listeners->find(BodyB) != Listeners->end();

			if (bHasListenerA || bHasListenerB)
			{
				EventQueue->push_back({ BodyA, BodyB, false });
			}
		}
	};

	static ContactListener s_ContactListener;

	PhysicsSubsystem::PhysicsSubsystem()
		: PhysicsWorld{ make_unique<b2World>(b2Vec2{ WEConfig.Physics.Gravity.x, WEConfig.Physics.Gravity.y }) }
		, PhysicsScale{ WEConfig.Physics.PhysicsScale }
		, VelocityIterations{ WEConfig.Physics.VelocityIterations }
		, PositionIterations{ WEConfig.Physics.PositionIterations }
		, CurrentWorld{ nullptr }
	{
		PhysicsWorld->SetAllowSleeping(false);
		s_ContactListener.Listeners = &ContactListeners;
		s_ContactListener.CurrentWorldPtr = &CurrentWorld;
		s_ContactListener.EventQueue = &ContactEventQueue;
		PhysicsWorld->SetContactListener(&s_ContactListener);
	}

	PhysicsSubsystem::~PhysicsSubsystem()
	{
	}

	void PhysicsSubsystem::Tick(float DeltaTime)
	{
		ProcessPendingDestruction();
		
		PhysicsWorld->Step(DeltaTime, VelocityIterations, PositionIterations);
		
		ProcessPendingDestruction();
		ProcessContactEvents();
	}

	void PhysicsSubsystem::ProcessContactEvents()
	{
		if (!CurrentWorld || ContactEventQueue.empty()) return;

		vector<ContactEvent> Events = ContactEventQueue;
		ContactEventQueue.clear();

		for (const auto& Event : Events)
		{
			// Handle BodyA
			auto ItA = ContactListeners.find(Event.BodyA);
			if (ItA != ContactListeners.end() && Event.BodyA->GetUserData().pointer)
			{
				auto* CollComp = reinterpret_cast<CollisionComponent*>(Event.BodyA->GetUserData().pointer);
				if (CollComp)
				{
					if (Event.bBegin)
						CollComp->OnComponentBeginOverlap(Event.BodyB);
					else
						CollComp->OnComponentEndOverlap(Event.BodyB);
				}
			}
			
			// Handle BodyB
			auto ItB = ContactListeners.find(Event.BodyB);
			if (ItB != ContactListeners.end() && Event.BodyB->GetUserData().pointer)
			{
				auto* CollComp = reinterpret_cast<CollisionComponent*>(Event.BodyB->GetUserData().pointer);
				if (CollComp)
				{
					if (Event.bBegin)
						CollComp->OnComponentBeginOverlap(Event.BodyA);
					else
						CollComp->OnComponentEndOverlap(Event.BodyA);
				}
			}
		}
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
			PendingDestruction.insert(Body);
		}
	}

	void PhysicsSubsystem::MarkForDestruction(b2Body* Body)
	{
		if (Body)
		{
			PendingDestruction.insert(Body);
		}
	}

	void PhysicsSubsystem::RegisterContactListener(b2Body* Body, ActorID ID)
	{
		if (Body && ID != INVALID_ACTOR_ID)
		{
			ContactListeners[Body] = ID;
		}
	}

	void PhysicsSubsystem::UnregisterContactListener(b2Body* Body)
	{
		if (Body)
		{
			ContactListeners.erase(Body);
		}
	}

	ActorID PhysicsSubsystem::GetBodyActorID(b2Body* Body) const
	{
		if (!Body) return INVALID_ACTOR_ID;
		auto It = ContactListeners.find(Body);
		if (It != ContactListeners.end())
		{
			return It->second;
		}
		return INVALID_ACTOR_ID;
	}

	void PhysicsSubsystem::ProcessPendingDestruction()
	{
		if (bInPhysicsStep)
		{
			return;
		}

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
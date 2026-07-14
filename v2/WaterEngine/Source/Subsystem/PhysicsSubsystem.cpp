// =============================================================================
// Water Engine v2.2.4
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
			QueueEvent(Contact, true);
		}

		void EndContact(b2Contact* Contact) override
		{
			QueueEvent(Contact, false);
		}

	private:
		// Resolve components + actor IDs NOW, while both b2Bodies are guaranteed
		// alive (Box2D is mid-callback). A body queued for destruction is freed
		// before the event queue drains, so nothing may be read through it later.
		// A dying body's userdata is already zeroed, leaving its component null:
		// its own callback is skipped, but its ID still reaches the other side so
		// the survivor can drop it from its overlap set.
		void QueueEvent(b2Contact* Contact, bool bBegin)
		{
			if (!Listeners || !CurrentWorldPtr || !*CurrentWorldPtr || !EventQueue) return;

			b2Body* BodyA = Contact->GetFixtureA()->GetBody();
			b2Body* BodyB = Contact->GetFixtureB()->GetBody();

			auto ItA = Listeners->find(BodyA);
			auto ItB = Listeners->find(BodyB);
			if (ItA == Listeners->end() && ItB == Listeners->end()) return;

			PhysicsSubsystem::ContactEvent Event;
			Event.bBegin = bBegin;
			if (ItA != Listeners->end())
			{
				Event.IDA   = ItA->second;
				Event.CompA = reinterpret_cast<CollisionComponent*>(BodyA->GetUserData().pointer);
			}
			if (ItB != Listeners->end())
			{
				Event.IDB   = ItB->second;
				Event.CompB = reinterpret_cast<CollisionComponent*>(BodyB->GetUserData().pointer);
			}
			EventQueue->push_back(Event);
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
		// No world to dispatch into: drop the events rather than let them
		// survive into the next world, where their components no longer exist.
		if (!CurrentWorld)
		{
			ContactEventQueue.clear();
			return;
		}
		if (ContactEventQueue.empty()) return;

		vector<ContactEvent> Events = ContactEventQueue;
		ContactEventQueue.clear();

		for (const auto& Event : Events)
		{
			if (Event.CompA && Event.IDB != INVALID_ACTOR_ID)
			{
				if (Event.bBegin)
					Event.CompA->OnComponentBeginOverlap(Event.IDB);
				else
					Event.CompA->OnComponentEndOverlap(Event.IDB);
			}

			if (Event.CompB && Event.IDA != INVALID_ACTOR_ID)
			{
				if (Event.bBegin)
					Event.CompB->OnComponentBeginOverlap(Event.IDA);
				else
					Event.CompB->OnComponentEndOverlap(Event.IDA);
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
			// DestroyBody fires EndContact synchronously; the listener map must
			// still hold this body so the event carries the dying actor's ID to
			// the other side (erasing first orphaned dead actors in overlap sets).
			PhysicsWorld->DestroyBody(Body);
			ContactListeners.erase(Body);
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
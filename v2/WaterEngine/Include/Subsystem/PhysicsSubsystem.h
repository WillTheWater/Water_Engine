// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Physics/IPhysicsContactListener.h"

class b2World;
class b2Body;
struct b2BodyDef;
struct b2Vec2;
class b2Contact;
class b2ContactListener;

namespace we
{
	class World;
	class CollisionComponent;
}

namespace we
{
	class PhysicsSubsystem
	{
	public:
		PhysicsSubsystem();
		~PhysicsSubsystem();

		void Tick(float DeltaTime);

		// World settings
		void SetGravity(vec2f Gravity);
		vec2f GetGravity() const;

		// Body management (used by components)
		b2Body* CreateBody(const b2BodyDef& Def);
		void DestroyBody(b2Body* Body);
		void MarkForDestruction(b2Body* Body);

		// Contact listener registration
		void RegisterContactListener(b2Body* Body, ActorID ID);
		void UnregisterContactListener(b2Body* Body);
		
		// Get ActorID for a body
		ActorID GetBodyActorID(b2Body* Body) const;
		
		// Set current world for actor lookup during contact callbacks. Queued
		// contact events reference components owned by the old world, so a
		// switch drops them.
		void SetCurrentWorld(World* InWorld) { CurrentWorld = InWorld; ContactEventQueue.clear(); }
		World* GetCurrentWorld() const { return CurrentWorld; }

		// Scale conversion (pixels <-> meters)
		float GetPhysicsScale() const { return PhysicsScale; }
		float PixelsToMeters(float Pixels) const { return Pixels * PhysicsScale; }
		float MetersToPixels(float Meters) const { return Meters / PhysicsScale; }

		vec2f PixelsToMeters(vec2f Pixels) const;
		vec2f MetersToPixels(vec2f Meters) const;

	private:
		void ProcessPendingDestruction();
		void ProcessContactEvents();

	private:
		unique<b2World> PhysicsWorld;
		float PhysicsScale;

		int VelocityIterations;
		int PositionIterations;

		set<b2Body*> PendingDestruction;
		dictionary<b2Body*, ActorID> ContactListeners;
		World* CurrentWorld = nullptr;
		bool bInPhysicsStep = false;
		bool bProcessingContactEvents = false;

	public:
		// Contact event queue - deferred to end of physics step. Everything is
		// resolved at capture time (inside the Box2D callback, while both bodies
		// are alive); events must never be read back through a b2Body, which may
		// be freed before the queue drains.
		struct ContactEvent
		{
			CollisionComponent* CompA = nullptr;  // null = side unregistered or being torn down
			CollisionComponent* CompB = nullptr;
			ActorID IDA = INVALID_ACTOR_ID;
			ActorID IDB = INVALID_ACTOR_ID;
			bool bBegin = true;  // true = BeginContact, false = EndContact
		};
	private:
		vector<ContactEvent> ContactEventQueue;
	};
}
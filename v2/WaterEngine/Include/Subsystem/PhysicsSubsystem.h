// =============================================================================
// Water Engine v2.0.0
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
		
		// Set current world for actor lookup during contact callbacks
		void SetCurrentWorld(World* InWorld) { CurrentWorld = InWorld; }
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
		// Contact event queue - deferred to end of physics step
		struct ContactEvent
		{
			b2Body* BodyA = nullptr;
			b2Body* BodyB = nullptr;
			bool bBegin = true;  // true = BeginContact, false = EndContact
		};
	private:
		vector<ContactEvent> ContactEventQueue;
	};
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

// Forward declarations for Box2D
class b2World;
class b2Body;
struct b2BodyDef;
struct b2Vec2;
class b2Contact;
class b2ContactListener;

namespace we
{
	// Contact listener interface for components
	class IPhysicsContactListener
	{
	public:
		virtual ~IPhysicsContactListener() = default;
		virtual void OnBeginContact(b2Body* OtherBody) = 0;
		virtual void OnEndContact(b2Body* OtherBody) = 0;
	};

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

		// Safe destruction during callbacks
		void MarkForDestruction(b2Body* Body);

		// Scale conversion (pixels <-> meters)
		float GetPhysicsScale() const { return PhysicsScale; }
		float PixelsToMeters(float Pixels) const { return Pixels * PhysicsScale; }
		float MetersToPixels(float Meters) const { return Meters / PhysicsScale; }

		vec2f PixelsToMeters(vec2f Pixels) const;
		vec2f MetersToPixels(vec2f Meters) const;

	private:
		void ProcessPendingDestruction();

	private:
		unique<b2World> PhysicsWorld;
		float PhysicsScale;

		int VelocityIterations;
		int PositionIterations;

		set<b2Body*> PendingDestruction;
	};
}
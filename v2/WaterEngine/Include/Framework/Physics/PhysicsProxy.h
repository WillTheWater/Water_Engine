// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/Physics/PhysicsTypes.h"

// Forward declare Box2D
class b2Body;
class b2Fixture;
class b2World;

namespace we
{
	// Forward declare
	class Actor;
	class PhysicsSubsystem;

	// =============================================================================
	// PhysicsProxyConfig - Creation parameters
	// =============================================================================
	struct PhysicsProxyConfig
	{
		// Physics simulation type
		PhysicsType BodyType = PhysicsType::Dynamic;
		
		// Collision filtering
		CollisionChannel CollisionChannel = CollisionChannel::WorldDynamic;
		uint CollisionMask = 0xFFFFFFFF;  // Collide with everything by default
		
		// Is this a sensor (overlap only, no physics response)?
		bool bSensor = false;
		
		// Simulation properties
		float LinearDamping = 0.0f;
		float AngularDamping = 0.0f;
		bool bFixedRotation = false;        // Prevent rotation?
		bool bBullet = false;               // CCD (Continuous Collision Detection)
		bool bSleepingAllowed = true;
		
		// Mass override (0 = auto-calculate from shapes)
		float Mass = 0.0f;
	};

	// =============================================================================
	// IPhysicsProxyCallback - Interface for collision events
	// =============================================================================
	class IPhysicsProxyCallback
	{
	public:
		virtual ~IPhysicsProxyCallback() = default;
		
		virtual void OnBeginOverlap(class PhysicsProxy* self, class PhysicsProxy* other) {}
		virtual void OnEndOverlap(class PhysicsProxy* self, class PhysicsProxy* other) {}
		virtual void OnHit(class PhysicsProxy* self, class PhysicsProxy* other, 
		                   const vec2f& normal, float impulse) {}
	};

	// =============================================================================
	// PhysicsProxy - Represents one physics body in the world
	// Owned by PhysicsSubsystem, referenced by Actor
	// =============================================================================
	class PhysicsProxy
	{
	public:
		// -------------------------------------------------------------------------
		// Lifecycle - Only PhysicsSubsystem creates/destroys these
		// -------------------------------------------------------------------------
		PhysicsProxy(Actor* owner, b2World* world, const PhysicsProxyConfig& config);
		~PhysicsProxy();
		
		// Non-copyable, non-movable (stable pointer required)
		PhysicsProxy(const PhysicsProxy&) = delete;
		PhysicsProxy& operator=(const PhysicsProxy&) = delete;
		PhysicsProxy(PhysicsProxy&&) = delete;
		PhysicsProxy& operator=(PhysicsProxy&&) = delete;

		// -------------------------------------------------------------------------
		// Ownership
		// -------------------------------------------------------------------------
		Actor* GetOwner() const { return Owner; }
		b2Body* GetBody() const { return Body; }
		
		// -------------------------------------------------------------------------
		// Shape Management - Add collision geometry
		// -------------------------------------------------------------------------
		void AddShape(const ShapeDefinition& shapeDef);
		void AddBox(vec2f halfSize, vec2f offset = { 0.0f, 0.0f });
		void AddCircle(float radius, vec2f offset = { 0.0f, 0.0f });
		void ClearShapes();  // Remove all fixtures
		
		uint GetShapeCount() const { return ShapeCount; }
		bool HasShapes() const { return ShapeCount > 0; }

		// -------------------------------------------------------------------------
		// Collision Filtering - Change what collides with what
		// -------------------------------------------------------------------------
		void SetCollisionChannel(we::CollisionChannel channel);
		void SetCollisionMask(uint mask);
		void SetCollisionResponse(we::CollisionChannel channel, bool bShouldCollide);
		
		we::CollisionChannel GetCollisionChannel() const { return Channel; }
		uint GetCollisionMask() const { return Mask; }
		
		// Sensor (trigger) control
		void SetSensor(bool bInSensor);
		bool IsSensor() const { return bSensor; }

		// -------------------------------------------------------------------------
		// Simulation Control
		// -------------------------------------------------------------------------
		void SetEnabled(bool bEnabled);
		bool IsEnabled() const { return bEnabled; }
		
		void SetSimulationType(PhysicsType type);
		PhysicsType GetSimulationType() const;
		
		void WakeUp();
		void PutToSleep();
		bool IsSleeping() const;

		// -------------------------------------------------------------------------
		// Transform (world space)
		// These are relative to Actor, but in physics world coordinates
		// -------------------------------------------------------------------------
		vec2f GetPosition() const;
		void SetPosition(const vec2f& pos);
		
		float GetRotation() const;  // Radians
		void SetRotation(float radians);
		
		vec2f GetLinearVelocity() const;
		void SetLinearVelocity(const vec2f& velocity);
		
		float GetAngularVelocity() const;
		void SetAngularVelocity(float velocity);

		// -------------------------------------------------------------------------
		// Forces & Impulses
		// -------------------------------------------------------------------------
		void AddForce(const vec2f& force);
		void AddForceAtLocation(const vec2f& force, const vec2f& location);
		void AddImpulse(const vec2f& impulse);
		void AddImpulseAtLocation(const vec2f& impulse, const vec2f& location);
		void AddTorque(float torque);
		
		// -------------------------------------------------------------------------
		// Events / Callbacks
		// -------------------------------------------------------------------------
		void SetCallback(IPhysicsProxyCallback* callback) { Callback = callback; }
		IPhysicsProxyCallback* GetCallback() const { return Callback; }
		
		// Internal event dispatch (called by PhysicsSubsystem)
		void DispatchBeginOverlap(PhysicsProxy* other);
		void DispatchEndOverlap(PhysicsProxy* other);
		void DispatchHit(PhysicsProxy* other, const vec2f& normal, float impulse);

		// -------------------------------------------------------------------------
		// Queries
		// -------------------------------------------------------------------------
		bool IsOverlapping(PhysicsProxy* other) const;
		vector<PhysicsProxy*> GetOverlappingProxies() const;
		
		// Raycast against this proxy only
		bool Raycast(const vec2f& start, const vec2f& end, vec2f& outHit, float& outFraction) const;

		// -------------------------------------------------------------------------
		// Internal (PhysicsSubsystem use only)
		// -------------------------------------------------------------------------
		void Internal_MarkForDestruction() { bPendingDestroy = true; }
		bool Internal_IsPendingDestroy() const { return bPendingDestroy; }
		void Internal_UpdateFilterData();  // Refresh collision filters

	private:
		// Owner
		Actor* Owner = nullptr;
		b2World* World = nullptr;
		b2Body* Body = nullptr;
		
		// Configuration
		we::CollisionChannel Channel;
		uint Mask;
		bool bSensor = false;
		bool bEnabled = true;
		bool bPendingDestroy = false;
		
		// State
		uint ShapeCount = 0;
		IPhysicsProxyCallback* Callback = nullptr;
		
		// Overlap tracking (for end overlap events)
		set<PhysicsProxy*> OverlappingProxies;
	};
}

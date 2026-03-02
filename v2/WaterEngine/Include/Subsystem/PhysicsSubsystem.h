// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"
#include "Framework/Physics/PhysicsTypes.h"
#include <box2d/b2_world.h>

namespace we
{
	class Actor;
	class PhysicsProxy;

	// =========================================================================
	// PhysicsContactListener - Routes Box2D events to PhysicsProxies
	// =========================================================================
	class PhysicsContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* Contact) override;
		void EndContact(b2Contact* Contact) override;
		void PreSolve(b2Contact* Contact, const b2Manifold* OldManifold) override;
	};

	// =========================================================================
	// PhysicsSubsystem - Manages the physics world and all PhysicsProxies
	// =========================================================================
	class PhysicsSubsystem
	{
	public:
		explicit PhysicsSubsystem(const EngineConfig::PhysicsConfig& Config);
		~PhysicsSubsystem();

		// =================================================================
		// World Simulation
		// =================================================================
		void Step(float DeltaTime);
		void SetGravity(const vec2f& Gravity);
		vec2f GetGravity() const;

		// =================================================================
		// Proxy Management
		// =================================================================
		PhysicsProxy* CreateProxy(Actor* Owner, const struct PhysicsProxyConfig& Config);
		void DestroyProxy(PhysicsProxy* Proxy);
		
		// Look up proxy from b2Body (used by contact listener)
		PhysicsProxy* FindProxy(b2Body* Body) const;
		PhysicsProxy* FindProxy(b2Fixture* Fixture) const;

		// =================================================================
		// Query Functions
		// =================================================================
		// Raycast - returns first hit proxy
		PhysicsProxy* Raycast(const vec2f& Start, const vec2f& End, 
		                      vec2f& OutHitPoint, vec2f& OutNormal) const;
		
		// Raycast all - returns all proxies along ray
		vector<std::pair<PhysicsProxy*, float>> RaycastAll(const vec2f& Start, 
		                                               const vec2f& End) const;
		
		// Overlap queries
		vector<PhysicsProxy*> OverlapCircle(const vec2f& Center, float Radius) const;
		vector<PhysicsProxy*> OverlapBox(const vec2f& Center, vec2f HalfSize) const;
		vector<PhysicsProxy*> OverlapPoint(const vec2f& Point) const;

		// =================================================================
		// Accessors
		// =================================================================
		float GetPhysicsScale() const { return PhysicsScale; }
		b2World* GetWorld() { return &PhysicsWorld; }
		const b2World* GetWorld() const { return &PhysicsWorld; }

	private:
		void ProcessPendingDestruction();

	private:
		b2World PhysicsWorld;
		float PhysicsScale;
		uint VelocityIterations;
		uint PositionIterations;

		PhysicsContactListener ContactListener;
		vector<unique<PhysicsProxy>> Proxies;
		vector<PhysicsProxy*> PendingDestruction;
	};
}

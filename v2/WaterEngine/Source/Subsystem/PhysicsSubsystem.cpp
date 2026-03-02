// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/PhysicsSubsystem.h"
#include "Framework/Physics/PhysicsProxy.h"
#include "Framework/World/Actor.h"
#include <algorithm>
#include "box2d/b2_body.h"
#include "box2d/b2_contact.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_world_callbacks.h"
#include "Utility/Log.h"

namespace we
{
	// =========================================================================
	// Raycast Callback for single hit
	// =========================================================================
	class SingleRaycastCallback : public b2RayCastCallback
	{
	public:
		b2Fixture* HitFixture = nullptr;
		b2Vec2 HitPoint;
		b2Vec2 HitNormal;
		float HitFraction = 1.0f;

		float ReportFixture(b2Fixture* Fixture, const b2Vec2& Point, 
		                   const b2Vec2& Normal, float Fraction) override
		{
			HitFixture = Fixture;
			HitPoint = Point;
			HitNormal = Normal;
			HitFraction = Fraction;
			return Fraction; // Stop at first hit
		}
	};

	// =========================================================================
	// Raycast Callback for all hits
	// =========================================================================
	class AllRaycastCallback : public b2RayCastCallback
	{
	public:
		vector<std::pair<b2Fixture*, float>> Hits;  // Fixture + fraction

		float ReportFixture(b2Fixture* Fixture, const b2Vec2& Point,
		                   const b2Vec2& Normal, float Fraction) override
		{
			Hits.push_back({ Fixture, Fraction });
			return 1.0f; // Continue raycast
		}
	};

	// =========================================================================
	// Query Callback for overlap tests
	// =========================================================================
	class OverlapQueryCallback : public b2QueryCallback
	{
	public:
		vector<b2Fixture*> Fixtures;

		bool ReportFixture(b2Fixture* Fixture) override
		{
			Fixtures.push_back(Fixture);
			return true; // Continue query
		}
	};

	// =========================================================================
	// PhysicsSubsystem Implementation
	// =========================================================================
	PhysicsSubsystem::PhysicsSubsystem(const EngineConfig::PhysicsConfig& Config)
		: PhysicsWorld{ b2Vec2{Config.Gravity.x, Config.Gravity.y} }
		, PhysicsScale{ Config.PhysicsScale }
		, VelocityIterations{ Config.VelocityIterations }
		, PositionIterations{ Config.PositionIterations }
		, ContactListener{}
	{
		PhysicsWorld.SetContactListener(&ContactListener);
		PhysicsWorld.SetAllowSleeping(true);
		LOG("PhysicsSubsystem initialized with gravity [{}, {}]", Config.Gravity.x, Config.Gravity.y);
	}

	PhysicsSubsystem::~PhysicsSubsystem() = default;

	void PhysicsSubsystem::Step(float DeltaTime)
	{
		// Process deferred destructions before physics step
		ProcessPendingDestruction();

		// Step the physics world
		PhysicsWorld.Step(DeltaTime, VelocityIterations, PositionIterations);
	}

	void PhysicsSubsystem::SetGravity(const vec2f& Gravity)
	{
		PhysicsWorld.SetGravity(b2Vec2{ Gravity.x, Gravity.y });
	}

	vec2f PhysicsSubsystem::GetGravity() const
	{
		b2Vec2 g = PhysicsWorld.GetGravity();
		return { g.x, g.y };
	}

	// =========================================================================
	// Proxy Management
	// =========================================================================
	PhysicsProxy* PhysicsSubsystem::CreateProxy(Actor* Owner, const PhysicsProxyConfig& Config)
	{
		if (!Owner)
		{
			ERROR("Cannot create PhysicsProxy with null owner!");
			return nullptr;
		}

		if (Owner->IsPendingDestroy())
		{
			ERROR("Cannot create PhysicsProxy for actor pending destroy!");
			return nullptr;
		}

		auto proxy = std::make_unique<PhysicsProxy>(Owner, &PhysicsWorld, Config);
		PhysicsProxy* ptr = proxy.get();
		Proxies.push_back(std::move(proxy));
		
		return ptr;
	}

	void PhysicsSubsystem::DestroyProxy(PhysicsProxy* Proxy)
	{
		if (!Proxy)
			return;

		Proxy->Internal_MarkForDestruction();
		PendingDestruction.push_back(Proxy);
	}

	void PhysicsSubsystem::ProcessPendingDestruction()
	{
		// Remove proxies marked for destruction
		if (!PendingDestruction.empty())
		{
			for (auto* proxy : PendingDestruction)
			{
				// Find and remove from Proxies vector
				auto it = std::remove_if(Proxies.begin(), Proxies.end(),
					[proxy](const unique<PhysicsProxy>& p) { return p.get() == proxy; });
				Proxies.erase(it, Proxies.end());
			}
			PendingDestruction.clear();
		}
	}

	PhysicsProxy* PhysicsSubsystem::FindProxy(b2Body* Body) const
	{
		if (!Body)
			return nullptr;
		
		// User data stores pointer to PhysicsProxy
		return reinterpret_cast<PhysicsProxy*>(Body->GetUserData().pointer);
	}

	PhysicsProxy* PhysicsSubsystem::FindProxy(b2Fixture* Fixture) const
	{
		if (!Fixture || !Fixture->GetBody())
			return nullptr;
		return FindProxy(Fixture->GetBody());
	}

	// =========================================================================
	// Query Functions
	// =========================================================================
	PhysicsProxy* PhysicsSubsystem::Raycast(const vec2f& Start, const vec2f& End,
	                                        vec2f& OutHitPoint, vec2f& OutNormal) const
	{
		SingleRaycastCallback callback;
		PhysicsWorld.RayCast(&callback, b2Vec2(Start.x, Start.y), b2Vec2(End.x, End.y));

		if (callback.HitFixture)
		{
			OutHitPoint = { callback.HitPoint.x, callback.HitPoint.y };
			OutNormal = { callback.HitNormal.x, callback.HitNormal.y };
			return FindProxy(callback.HitFixture->GetBody());
		}

		return nullptr;
	}

	vector<std::pair<PhysicsProxy*, float>> PhysicsSubsystem::RaycastAll(const vec2f& Start, 
	                                                               const vec2f& End) const
	{
		vector<std::pair<PhysicsProxy*, float>> results;
		
		AllRaycastCallback callback;
		PhysicsWorld.RayCast(&callback, b2Vec2(Start.x, Start.y), b2Vec2(End.x, End.y));

		// Sort by distance (fraction)
		std::sort(callback.Hits.begin(), callback.Hits.end(),
			[](const auto& a, const auto& b) { return a.second < b.second; });

		for (const auto& [fixture, fraction] : callback.Hits)
		{
			if (auto* proxy = FindProxy(fixture->GetBody()))
			{
				results.push_back({ proxy, fraction });
			}
		}

		return results;
	}

	vector<PhysicsProxy*> PhysicsSubsystem::OverlapCircle(const vec2f& Center, float Radius) const
	{
		vector<PhysicsProxy*> results;
		
		b2AABB aabb;
		aabb.lowerBound.Set(Center.x - Radius, Center.y - Radius);
		aabb.upperBound.Set(Center.x + Radius, Center.y + Radius);

		OverlapQueryCallback callback;
		PhysicsWorld.QueryAABB(&callback, aabb);

		// Filter by actual circle overlap
		for (auto* fixture : callback.Fixtures)
		{
			// Get closest point on shape to circle center
			b2Vec2 closest = fixture->GetAABB(0).GetCenter(); // Simplified
			float distSq = b2DistanceSquared(closest, b2Vec2(Center.x, Center.y));
			
			if (distSq <= Radius * Radius)
			{
				if (auto* proxy = FindProxy(fixture->GetBody()))
				{
					results.push_back(proxy);
				}
			}
		}

		return results;
	}

	vector<PhysicsProxy*> PhysicsSubsystem::OverlapBox(const vec2f& Center, vec2f HalfSize) const
	{
		vector<PhysicsProxy*> results;
		
		b2AABB aabb;
		aabb.lowerBound.Set(Center.x - HalfSize.x, Center.y - HalfSize.y);
		aabb.upperBound.Set(Center.x + HalfSize.x, Center.y + HalfSize.y);

		OverlapQueryCallback callback;
		PhysicsWorld.QueryAABB(&callback, aabb);

		for (auto* fixture : callback.Fixtures)
		{
			if (auto* proxy = FindProxy(fixture->GetBody()))
			{
				results.push_back(proxy);
			}
		}

		return results;
	}

	vector<PhysicsProxy*> PhysicsSubsystem::OverlapPoint(const vec2f& Point) const
	{
		vector<PhysicsProxy*> results;
		
		// Small AABB around point
		b2AABB aabb;
		aabb.lowerBound.Set(Point.x - 0.001f, Point.y - 0.001f);
		aabb.upperBound.Set(Point.x + 0.001f, Point.y + 0.001f);

		OverlapQueryCallback callback;
		PhysicsWorld.QueryAABB(&callback, aabb);

		// Check if point is actually inside fixture
		for (auto* fixture : callback.Fixtures)
		{
			if (fixture->TestPoint(b2Vec2(Point.x, Point.y)))
			{
				if (auto* proxy = FindProxy(fixture->GetBody()))
				{
					results.push_back(proxy);
				}
			}
		}

		return results;
	}

	// =========================================================================
	// Contact Listener
	// =========================================================================
	void PhysicsContactListener::BeginContact(b2Contact* Contact)
	{
		b2Fixture* FixtureA = Contact->GetFixtureA();
		b2Fixture* FixtureB = Contact->GetFixtureB();
		b2Body* BodyA = FixtureA->GetBody();
		b2Body* BodyB = FixtureB->GetBody();

		// Get proxies from user data
		auto* ProxyA = reinterpret_cast<PhysicsProxy*>(BodyA->GetUserData().pointer);
		auto* ProxyB = reinterpret_cast<PhysicsProxy*>(BodyB->GetUserData().pointer);

		if (ProxyA && !ProxyA->Internal_IsPendingDestroy())
		{
			ProxyA->DispatchBeginOverlap(ProxyB);
		}

		if (ProxyB && !ProxyB->Internal_IsPendingDestroy())
		{
			ProxyB->DispatchBeginOverlap(ProxyA);
		}
	}

	void PhysicsContactListener::EndContact(b2Contact* Contact)
	{
		b2Fixture* FixtureA = Contact->GetFixtureA();
		b2Fixture* FixtureB = Contact->GetFixtureB();
		b2Body* BodyA = FixtureA->GetBody();
		b2Body* BodyB = FixtureB->GetBody();

		auto* ProxyA = reinterpret_cast<PhysicsProxy*>(BodyA->GetUserData().pointer);
		auto* ProxyB = reinterpret_cast<PhysicsProxy*>(BodyB->GetUserData().pointer);

		if (ProxyA && !ProxyA->Internal_IsPendingDestroy())
		{
			ProxyA->DispatchEndOverlap(ProxyB);
		}

		if (ProxyB && !ProxyB->Internal_IsPendingDestroy())
		{
			ProxyB->DispatchEndOverlap(ProxyA);
		}
	}

	void PhysicsContactListener::PreSolve(b2Contact* Contact, const b2Manifold* OldManifold)
	{
		// Can disable collision here based on game logic
		// For now, just let everything collide normally
	}
}

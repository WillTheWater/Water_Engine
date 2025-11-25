#include "Framework/PhysicsSystem.h"
#include "Framework/Actor.h"
#include "Framework/Core.h"

namespace we
{
	unique<PhysicsSystem> PhysicsSystem::Instance{ nullptr };

	PhysicsSystem& PhysicsSystem::Get()
	{
		if (!Instance)
		{
			Instance = unique<PhysicsSystem>(new PhysicsSystem());
		}
		return *Instance;
	}

	PhysicsSystem::PhysicsSystem()
		: PhysicsScale{ 0.01f }, // 100 pixels = 1 meter
		VelocityIterations{ 4 }
	{
		LOG("PhysicsSystem: --- Initializing Box2D v3.1 ---");

		// 1. Get the default definition (CRITICAL in v3)
		WorldDef = b2DefaultWorldDef();

		// 2. Customize gravity (optional, but good for testing)
		WorldDef.gravity = { 0.0f, 9.8f };

		// 3. Create the world handle
		PhysicsWorld = b2CreateWorld(&WorldDef);

		if (b2World_IsValid(PhysicsWorld))
		{
			LOG("PhysicsSystem: [SUCCESS] World Created. ID: %d", PhysicsWorld.index1);
		}
		else
		{
			LOG("PhysicsSystem: [FATAL] Failed to create World.");
		}
	}

	PhysicsSystem::~PhysicsSystem()
	{
		if (b2World_IsValid(PhysicsWorld))
		{
			b2DestroyWorld(PhysicsWorld);
			LOG("PhysicsSystem: World Destroyed.");
		}
	}

	void PhysicsSystem::Step(float DeltaTime)
	{
		if (!b2World_IsValid(PhysicsWorld)) return;

		// The v3 Step function
		b2World_Step(PhysicsWorld, DeltaTime, VelocityIterations);
	}

	bool PhysicsSystem::IsWorldValid() const
	{
		return b2World_IsValid(PhysicsWorld);
	}

	b2BodyId PhysicsSystem::AddListener(Actor* Listener)
	{
		if (!Listener || Listener->IsPendingDestroy() || !IsWorldValid()) { return b2_nullBodyId; }

		float pScale = GetPhysicsScale();

		// --- 1. Define and Create Body ---
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody; // Start with dynamic body for movement/gravity testing
		bodyDef.position = {
			Listener->GetActorLocation().x * pScale,
			Listener->GetActorLocation().y * pScale
		};
		bodyDef.rotation = b2MakeRot(Listener->GetActorRotation().asRadians());

		// Store the Actor pointer in Box2D's userData for retrieval during synchronization/collisions
		bodyDef.userData = Listener;

		b2BodyId bodyId = b2CreateBody(PhysicsWorld, &bodyDef);

		if (!b2Body_IsValid(bodyId))
		{
			// FIX: Removed GetName().c_str() since Actor doesn't define GetName() yet.
			LOG("PhysicsSystem: Failed to create body for an Actor.");
			return b2_nullBodyId;
		}

		// --- 2. Define and Create Shape (Collider) ---
		sf::FloatRect bounds = Listener->GetSpriteBounds();

		// Box2D uses half-extents, so divide the sprite bounds by 2, and then scale.
		b2Polygon boxShape = b2MakeBox(
			bounds.size.x * 0.5f * pScale,
			bounds.size.y * 0.5f * pScale
		);

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.material = b2DefaultSurfaceMaterial();

		// Set to false for solid collision/gravity effects
		shapeDef.isSensor = false;
		shapeDef.userData = Listener;

		b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

		// --- 3. Track Body ---
		Bodies.push_back(bodyId);
		// FIX: Removed GetName().c_str() since Actor doesn't define GetName() yet.
		LOG("PhysicsSystem: Added body %u for an Actor.", bodyId.index1);

		return bodyId;
	}

	void PhysicsSystem::RemoveListener(b2BodyId PhysicsBodyToRemove)
	{
		if (!b2Body_IsValid(PhysicsBodyToRemove) || !IsWorldValid()) { return; }

		// 1. Remove from our tracking vector (CRITICAL for v3.1)
		// Replaced std::erase_if (C++20) with C++17 Erase-Remove Idiom
		Bodies.erase(
			std::remove_if(Bodies.begin(), Bodies.end(), [&](b2BodyId id) {
				return id.index1 == PhysicsBodyToRemove.index1;
				}),
			Bodies.end()
		);

		// 2. Destroy the body in the Box2D world
		b2DestroyBody(PhysicsBodyToRemove);
		LOG("PhysicsSystem: Destroyed body %u.", PhysicsBodyToRemove.index1);
	}
}
#include "Framework/PhysicsSystem.h"
#include "Framework/Actor.h"

namespace we
{
	unique<PhysicsSystem> PhysicsSystem::PhysicsSysm{ nullptr };

	PhysicsSystem::PhysicsSystem()
		: PhysicsScale{ 0.1f },
		VelocityIteration{4}
	{
		WorldDef = b2DefaultWorldDef();
		WorldDef.gravity = { 0.0f, 0.f }; // { 0.f, -9.8f } Default Gravity
		PhysicsWorld = b2CreateWorld(&WorldDef);
	}

	PhysicsSystem& PhysicsSystem::GetPhysiscSystem()
	{
		if (!PhysicsSysm)
		{
			PhysicsSysm = std::move(unique<PhysicsSystem>{new PhysicsSystem});
		}
		return *PhysicsSysm;
	}

	void PhysicsSystem::Step(float DeltaTime)
	{
		if (!b2World_IsValid(PhysicsWorld)) { return; } 

		int subStepCount = VelocityIteration; 
		b2World_Step(PhysicsWorld, DeltaTime, subStepCount);
	}

	b2BodyId PhysicsSystem::AddListener(Actor* Listener)
	{
		if (!Listener || Listener->IsPendingDestroy()) { return b2_nullBodyId; }

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = {
            Listener->GetActorLocation().x * GetPhysicsScale(),
            Listener->GetActorLocation().y * GetPhysicsScale()
        };

        bodyDef.rotation = b2MakeRot((Listener->GetActorRotation().asRadians()));

        bodyDef.userData = Listener;

        b2BodyId bodyId = b2CreateBody(PhysicsWorld, &bodyDef);

        if (!b2Body_IsValid(bodyId))
            return b2_nullBodyId;

        auto bounds = Listener->GetSpriteBounds();
        b2Polygon boxShape = b2MakeBox(
            bounds.size.x * 0.5f * GetPhysicsScale(),
            bounds.size.y * 0.5f * GetPhysicsScale()
        );

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.material = b2DefaultSurfaceMaterial();
        shapeDef.isSensor = true;
        shapeDef.userData = Listener;

        b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

        return bodyId;
	}
	void PhysicsSystem::RemoveListener(b2BodyId PhysicsBodyToRemove)
	{
		//TODO: Remove PB
	}
}
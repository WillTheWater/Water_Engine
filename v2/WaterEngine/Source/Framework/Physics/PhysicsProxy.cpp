// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/Physics/PhysicsProxy.h"
#include "Framework/World/Actor.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_world.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
#include "Utility/Log.h"

namespace we
{
	// =========================================================================
	// Constructor / Destructor
	// =========================================================================
	PhysicsProxy::PhysicsProxy(Actor* owner, b2World* world, const PhysicsProxyConfig& config)
		: Owner(owner)
		, World(world)
		, Channel(config.CollisionChannel)
		, Mask(config.CollisionMask)
		, bSensor(config.bSensor)
		, bEnabled(true)
		, bPendingDestroy(false)
		, ShapeCount(0)
		, Callback(nullptr)
	{
		if (!World)
		{
			ERROR("PhysicsProxy created with null world!");
			return;
		}

		// Create body definition
		b2BodyDef bodyDef;
		
		// Map PhysicsType to b2BodyType
		switch (config.BodyType)
		{
		case PhysicsType::Static:
			bodyDef.type = b2_staticBody;
			break;
		case PhysicsType::Kinematic:
			bodyDef.type = b2_kinematicBody;
			break;
		case PhysicsType::Dynamic:
		default:
			bodyDef.type = b2_dynamicBody;
			break;
		}

		// Set initial properties
		bodyDef.linearDamping = config.LinearDamping;
		bodyDef.angularDamping = config.AngularDamping;
		bodyDef.fixedRotation = config.bFixedRotation;
		bodyDef.bullet = config.bBullet;
		bodyDef.allowSleep = config.bSleepingAllowed;
		
		// User data points back to this proxy
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

		// Create the body
		Body = World->CreateBody(&bodyDef);
		
		if (!Body)
		{
			ERROR("Failed to create b2Body in PhysicsProxy!");
			return;
		}

		// Apply initial filter data
		Internal_UpdateFilterData();
	}

	PhysicsProxy::~PhysicsProxy()
	{
		if (Body && World)
		{
			World->DestroyBody(Body);
			Body = nullptr;
		}
	}

	// =========================================================================
	// Shape Management
	// =========================================================================
	void PhysicsProxy::AddShape(const ShapeDefinition& shapeDef)
	{
		if (!Body)
		{
			ERROR("Cannot add shape - body is null!");
			return;
		}

		b2FixtureDef fixtureDef;
		
		// Set up collision filtering
		fixtureDef.filter.categoryBits = static_cast<uint16>(Channel);
		fixtureDef.filter.maskBits = static_cast<uint16>(Mask);
		
		// Sensors don't generate contact responses
		fixtureDef.isSensor = bSensor;

		switch (shapeDef.ShapeType)
		{
		case ShapeDefinition::Type::Box:
		{
			b2PolygonShape shape;
			shape.SetAsBox(
				shapeDef.HalfSize.x,
				shapeDef.HalfSize.y,
				b2Vec2(shapeDef.LocalOffset.x, shapeDef.LocalOffset.y),
				shapeDef.LocalRotation
			);
			fixtureDef.shape = &shape;
			Body->CreateFixture(&fixtureDef);
			ShapeCount++;
			break;
		}
		case ShapeDefinition::Type::Circle:
		{
			b2CircleShape shape;
			shape.m_radius = shapeDef.Radius;
			shape.m_p.Set(shapeDef.LocalOffset.x, shapeDef.LocalOffset.y);
			fixtureDef.shape = &shape;
			Body->CreateFixture(&fixtureDef);
			ShapeCount++;
			break;
		}
		case ShapeDefinition::Type::Polygon:
		{
			if (shapeDef.VertexCount < 3 || shapeDef.VertexCount > ShapeDefinition::MaxPolygonVertices)
			{
				ERROR("Invalid polygon vertex count: {}", shapeDef.VertexCount);
				return;
			}
			
			b2PolygonShape shape;
			b2Vec2 vertices[ShapeDefinition::MaxPolygonVertices];
			for (uint i = 0; i < shapeDef.VertexCount; i++)
			{
				vertices[i].Set(shapeDef.Vertices[i].x, shapeDef.Vertices[i].y);
			}
			shape.Set(vertices, static_cast<int32>(shapeDef.VertexCount));
			fixtureDef.shape = &shape;
			Body->CreateFixture(&fixtureDef);
			ShapeCount++;
			break;
		}
		}
	}

	void PhysicsProxy::AddBox(vec2f halfSize, vec2f offset)
	{
		AddShape(ShapeDefinition::MakeBox(halfSize, offset));
	}

	void PhysicsProxy::AddCircle(float radius, vec2f offset)
	{
		AddShape(ShapeDefinition::MakeCircle(radius, offset));
	}

	void PhysicsProxy::ClearShapes()
	{
		if (!Body)
			return;

		b2Fixture* fixture = Body->GetFixtureList();
		while (fixture)
		{
			b2Fixture* next = fixture->GetNext();
			Body->DestroyFixture(fixture);
			fixture = next;
		}
		ShapeCount = 0;
	}

	// =========================================================================
	// Collision Filtering
	// =========================================================================
	void PhysicsProxy::SetCollisionChannel(CollisionChannel channel)
	{
		Channel = channel;
		Internal_UpdateFilterData();
	}

	void PhysicsProxy::SetCollisionMask(uint mask)
	{
		Mask = mask;
		Internal_UpdateFilterData();
	}

	void PhysicsProxy::SetCollisionResponse(CollisionChannel channel, bool bShouldCollide)
	{
		uint bit = static_cast<uint>(channel);
		if (bShouldCollide)
			Mask |= bit;
		else
			Mask &= ~bit;
		
		Internal_UpdateFilterData();
	}

	void PhysicsProxy::SetSensor(bool bInSensor)
	{
		if (bSensor == bInSensor)
			return;
		
		bSensor = bInSensor;
		
		// Update all fixtures
		if (Body)
		{
			b2Fixture* fixture = Body->GetFixtureList();
			while (fixture)
			{
				fixture->SetSensor(bSensor);
				fixture = fixture->GetNext();
			}
		}
	}

	void PhysicsProxy::Internal_UpdateFilterData()
	{
		if (!Body)
			return;

		b2Fixture* fixture = Body->GetFixtureList();
		while (fixture)
		{
			b2Filter filter = fixture->GetFilterData();
			filter.categoryBits = static_cast<uint16>(Channel);
			filter.maskBits = static_cast<uint16>(Mask);
			fixture->SetFilterData(filter);
			fixture = fixture->GetNext();
		}
	}

	// =========================================================================
	// Simulation Control
	// =========================================================================
	void PhysicsProxy::SetEnabled(bool bInEnabled)
	{
		bEnabled = bInEnabled;
		if (Body)
		{
			Body->SetEnabled(bEnabled);
		}
	}

	void PhysicsProxy::SetSimulationType(PhysicsType type)
	{
		if (!Body)
			return;

		b2BodyType bodyType;
		switch (type)
		{
		case PhysicsType::Static:
			bodyType = b2_staticBody;
			break;
		case PhysicsType::Kinematic:
			bodyType = b2_kinematicBody;
			break;
		case PhysicsType::Dynamic:
		default:
			bodyType = b2_dynamicBody;
			break;
		}
		Body->SetType(bodyType);
	}

	PhysicsType PhysicsProxy::GetSimulationType() const
	{
		if (!Body)
			return PhysicsType::None;

		switch (Body->GetType())
		{
		case b2_staticBody:
			return PhysicsType::Static;
		case b2_kinematicBody:
			return PhysicsType::Kinematic;
		case b2_dynamicBody:
			return PhysicsType::Dynamic;
		default:
			return PhysicsType::None;
		}
	}

	void PhysicsProxy::WakeUp()
	{
		if (Body)
			Body->SetAwake(true);
	}

	void PhysicsProxy::PutToSleep()
	{
		if (Body)
			Body->SetAwake(false);
	}

	bool PhysicsProxy::IsSleeping() const
	{
		if (!Body)
			return true;
		return !Body->IsAwake();
	}

	// =========================================================================
	// Transform
	// =========================================================================
	vec2f PhysicsProxy::GetPosition() const
	{
		if (!Body)
			return { 0.0f, 0.0f };
		
		b2Vec2 pos = Body->GetPosition();
		return { pos.x, pos.y };
	}

	void PhysicsProxy::SetPosition(const vec2f& pos)
	{
		if (Body)
		{
			Body->SetTransform(b2Vec2(pos.x, pos.y), Body->GetAngle());
		}
	}

	float PhysicsProxy::GetRotation() const
	{
		if (!Body)
			return 0.0f;
		return Body->GetAngle();
	}

	void PhysicsProxy::SetRotation(float radians)
	{
		if (Body)
		{
			Body->SetTransform(Body->GetPosition(), radians);
		}
	}

	vec2f PhysicsProxy::GetLinearVelocity() const
	{
		if (!Body)
			return { 0.0f, 0.0f };
		
		b2Vec2 vel = Body->GetLinearVelocity();
		return { vel.x, vel.y };
	}

	void PhysicsProxy::SetLinearVelocity(const vec2f& velocity)
	{
		if (Body)
		{
			Body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
		}
	}

	float PhysicsProxy::GetAngularVelocity() const
	{
		if (!Body)
			return 0.0f;
		return Body->GetAngularVelocity();
	}

	void PhysicsProxy::SetAngularVelocity(float velocity)
	{
		if (Body)
		{
			Body->SetAngularVelocity(velocity);
		}
	}

	// =========================================================================
	// Forces & Impulses
	// =========================================================================
	void PhysicsProxy::AddForce(const vec2f& force)
	{
		if (Body)
		{
			Body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
		}
	}

	void PhysicsProxy::AddForceAtLocation(const vec2f& force, const vec2f& location)
	{
		if (Body)
		{
			Body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(location.x, location.y), true);
		}
	}

	void PhysicsProxy::AddImpulse(const vec2f& impulse)
	{
		if (Body)
		{
			Body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
		}
	}

	void PhysicsProxy::AddImpulseAtLocation(const vec2f& impulse, const vec2f& location)
	{
		if (Body)
		{
			Body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(location.x, location.y), true);
		}
	}

	void PhysicsProxy::AddTorque(float torque)
	{
		if (Body)
		{
			Body->ApplyTorque(torque, true);
		}
	}

	// =========================================================================
	// Event Dispatch
	// =========================================================================
	void PhysicsProxy::DispatchBeginOverlap(PhysicsProxy* other)
	{
		if (!other)
			return;

		OverlappingProxies.insert(other);

		if (Callback)
		{
			Callback->OnBeginOverlap(this, other);
		}
	}

	void PhysicsProxy::DispatchEndOverlap(PhysicsProxy* other)
	{
		if (!other)
			return;

		OverlappingProxies.erase(other);

		if (Callback)
		{
			Callback->OnEndOverlap(this, other);
		}
	}

	void PhysicsProxy::DispatchHit(PhysicsProxy* other, const vec2f& normal, float impulse)
	{
		if (Callback)
		{
			Callback->OnHit(this, other, normal, impulse);
		}
	}

	// =========================================================================
	// Queries
	// =========================================================================
	bool PhysicsProxy::IsOverlapping(PhysicsProxy* other) const
	{
		if (!other)
			return false;
		return OverlappingProxies.find(other) != OverlappingProxies.end();
	}

	vector<PhysicsProxy*> PhysicsProxy::GetOverlappingProxies() const
	{
		vector<PhysicsProxy*> result;
		result.reserve(OverlappingProxies.size());
		for (auto* proxy : OverlappingProxies)
		{
			result.push_back(proxy);
		}
		return result;
	}

	bool PhysicsProxy::Raycast(const vec2f& start, const vec2f& end, vec2f& outHit, float& outFraction) const
	{
		if (!Body)
			return false;

		// Simple raycast against body AABB first
		b2AABB aabb = Body->GetFixtureList()->GetAABB(0);
		b2RayCastInput input;
		input.p1.Set(start.x, start.y);
		input.p2.Set(end.x, end.y);
		input.maxFraction = 1.0f;

		b2RayCastOutput output;
		bool hit = false;

		b2Fixture* fixture = Body->GetFixtureList();
		while (fixture)
		{
			if (fixture->RayCast(&output, input, 0))
			{
				if (output.fraction < outFraction)
				{
					outFraction = output.fraction;
					outHit = { output.normal.x, output.normal.y };
					hit = true;
				}
			}
			fixture = fixture->GetNext();
		}

		return hit;
	}
}

#include "Framework/PhysicsSystem.h"

namespace we
{
	unique<PhysicsSystem> PhysicsSystem::PhysicsSysm{ nullptr };

	PhysicsSystem& PhysicsSystem::GetPhysiscSystem()
	{
		if (!PhysicsSysm)
		{
			PhysicsSysm = std::move(unique<PhysicsSystem>{new PhysicsSystem});
		}
		return *PhysicsSysm;
	}
	PhysicsSystem::PhysicsSystem()
		: PhysicsScale{ 0.1f }
	{
		WorldDef.gravity = { 0.0f, 0.f }; // { 0.f, -9.8f } Default Gravity
		PhysicsWorld = b2CreateWorld(&WorldDef);
	}
}
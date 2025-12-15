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
			Instance = std::move(unique<PhysicsSystem>{new PhysicsSystem});
		}
		return *Instance;
	}
	PhysicsSystem::PhysicsSystem()
	{
	}
}
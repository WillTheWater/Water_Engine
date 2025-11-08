#pragma once

#include <box2d/box2d.h>
#include "Framework/Core.h"

namespace we
{
	class PhysicsSystem
	{
	public:
		static PhysicsSystem& GetPhysiscSystem();

	protected:
		PhysicsSystem();

	private:
		static unique<PhysicsSystem> PhysicsSysm;
		b2WorldId PhysicsWorld;
		b2WorldDef WorldDef;
		float PhysicsScale;
	};
}
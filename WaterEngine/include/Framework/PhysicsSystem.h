#pragma once

#include <box2d/box2d.h>
#include "Framework/Core.h"

namespace we
{
	class Actor;

	class PhysicsSystem
	{
	public:
		static PhysicsSystem& GetPhysiscSystem();

		void Step(float DeltaTime);
		b2BodyId AddListener(Actor* Listener);
		void RemoveListener(b2BodyId PhysicsBodyToRemove);
		float GetPhysicsScale() const { return PhysicsScale; }

	protected:
		PhysicsSystem();

	private:
		static unique<PhysicsSystem> PhysicsSysm;
		b2WorldId PhysicsWorld;
		b2WorldDef WorldDef;
		float PhysicsScale;
		int VelocityIteration;
	};
}
#pragma once
#include <box2d/box2d.h>
#include "Framework/Core.h"

namespace we
{
	class Actor;

	class PhysicsSystem
	{
	public:
		static PhysicsSystem& Get();

		void Step(float DeltaTime);
		bool IsWorldValid() const;

		~PhysicsSystem();

		// --- Body Management ---
		b2BodyId AddListener(Actor* Listener);
		void RemoveListener(b2BodyId PhysicsBodyToRemove);

		// --- Getters ---
		float GetPhysicsScale() const { return PhysicsScale; }
		int GetVelocityIterations() const { return VelocityIterations; }

	private:
		PhysicsSystem();

		static unique<PhysicsSystem> Instance;

		b2WorldId PhysicsWorld;
		b2WorldDef WorldDef;
		float PhysicsScale;
		int VelocityIterations;

		std::vector<b2BodyId> Bodies;
		b2ContactEvents PhysicsEvents;
	};
}
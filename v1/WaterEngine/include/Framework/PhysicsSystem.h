#pragma once
#include <box2d/b2_world.h>
#include "Framework/Core.h"

namespace we
{
	class Actor;

	class PhysicsContactListener : public b2ContactListener
	{
		virtual void BeginContact(b2Contact* Contact) override;
		virtual void EndContact(b2Contact* Contact) override;
	};

	class PhysicsSystem
	{
	public:
		static PhysicsSystem& Get();

		void Step(float DeltaTime);
		b2Body* AddListener(Actor* Listener);
		void RemoveListener(b2Body* LisenterBody);
		void UpdateBodyCollision(Actor* Listener);
		float GetPhysicsScale() const { return PhysicsScale; }
		static void Cleanup();

	protected:
		PhysicsSystem();

	private:
		static unique<PhysicsSystem> Instance;
		b2World PhysicsWorld;
		float PhysicsScale;
		int VelocityIterations;
		int PositionIterations;

		PhysicsContactListener ContactListener;
		Set<b2Body*> PendingRemovalListeners;
		void RemovePendingListeners();
	};
}
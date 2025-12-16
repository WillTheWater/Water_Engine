#pragma once
#include "Spaceship/Spaceship.h"

namespace we
{
	class EnemySpaceship : public Spaceship
	{
	public:
		EnemySpaceship(World* OwningWorld, const string& TexturePath, float CollisionDamage = 1000.f);

		virtual void Tick(float DeltTime) override;

	private:
		virtual void OnActorBeginOverlap(Actor* OtherActor) override;
		float CollisionDamage;
	};
}
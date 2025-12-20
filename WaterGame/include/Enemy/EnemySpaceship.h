#pragma once
#include "Spaceship/Spaceship.h"
#include "Gameplay/Pickups.h"

namespace we
{
	class EnemySpaceship : public Spaceship
	{
	public:
		EnemySpaceship(World* OwningWorld
			, const string& TexturePath
			, float CollisionDamage = 1000.f
			, const List<PickupFactoryFunction> Pickups = {
				SpawnHealthPickup,
				SpawnTripleShotPickup,
				SpawnMegaShotPickup
			}
		);

		virtual void Tick(float DeltaTime) override;

	private:
		virtual void SpawnDrops() override;
		virtual void OnActorBeginOverlap(Actor* OtherActor) override;
		float CollisionDamage;

		List<PickupFactoryFunction> Pickups;
	};
}
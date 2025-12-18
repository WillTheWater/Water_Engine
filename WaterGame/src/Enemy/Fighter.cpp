#include "Enemy/Fighter.h"
#include "Weapons/BulletShooter.h"

namespace we
{
	Fighter::Fighter(World* OwningWorld, const string TexturePath, const sf::Vector2f& Velocity)
		: EnemySpaceship{OwningWorld, TexturePath}
		, Shooter{ new BulletShooter{this} }
	{
		//SetVelocity(Velocity);
		SetLocalForwardVector(sf::Vector2f{0, 1});
	}

	void Fighter::Tick(float DeltaTime)
	{
		EnemySpaceship::Tick(DeltaTime);
		Shoot();
	}

	void Fighter::Shoot()
	{
		Shooter->Shoot();
	}
}
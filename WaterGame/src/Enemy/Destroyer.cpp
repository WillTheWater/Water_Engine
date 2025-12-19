#include "Enemy/Destroyer.h"
#include "Weapons/BulletShooter.h"

namespace we
{
	Destroyer::Destroyer(World* OwningWorld, const string TexturePath, const sf::Vector2f& Velocity)
		: EnemySpaceship{OwningWorld, TexturePath}
		, Shooter1{ new BulletShooter{this, 0.8, {20.f, 40.f}} }
		, Shooter2{ new BulletShooter{this, 0.8, {-20.f, 40.f}} }
		, Shooter3{ new BulletShooter{this, 0.8, {100, 50.f}} }
		, Shooter4{ new BulletShooter{this, 0.8, {-100, 50.f}} }

	{
		SetVelocity(Velocity);
		SetLocalForwardVector(sf::Vector2f{ 0, 1 });
	}

	void Destroyer::Tick(float DeltaTime)
	{
		EnemySpaceship::Tick(DeltaTime);
		Shoot();
	}

	void Destroyer::Shoot()
	{
		Shooter1->Shoot();
		Shooter2->Shoot();
		Shooter3->Shoot();
		Shooter4->Shoot();
	}
}
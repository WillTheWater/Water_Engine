#pragma once
#include "Enemy/EnemySpaceship.h"

namespace we
{
	class BulletShooter;

	class Destroyer : public EnemySpaceship
	{
	public:
		Destroyer(World* OwningWorld, const string TexturePath = "SpaceShooterRedux/PNG/Enemies/enemyBlue4.png", const sf::Vector2f& Velocity = { 0, 15 });

		virtual void Tick(float DeltaTime) override;

	private:
		virtual void Shoot() override;
		unique<BulletShooter> Shooter1;
		unique<BulletShooter> Shooter2;
		unique<BulletShooter> Shooter3;
		unique<BulletShooter> Shooter4;
	};
}
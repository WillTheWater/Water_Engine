#pragma once
#include "Enemy/EnemySpaceship.h"

namespace we
{
	class BulletShooter;

	class Fighter : public EnemySpaceship
	{
	public:
		Fighter(World* OwningWorld, const string TexturePath = "SpaceShooterRedux/PNG/Enemies/enemyRed1.png", const sf::Vector2f& Velocity = {0, 60});

		virtual void Tick(float DeltaTime) override;

		void OscillateX(float DeltaTime);

	private:
		virtual void Shoot() override;
		unique<BulletShooter> Shooter;
		float OscTime;
	};
}
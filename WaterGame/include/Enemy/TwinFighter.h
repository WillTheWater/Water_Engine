#pragma once
#include "Enemy/EnemySpaceship.h"

namespace we
{
	class BulletShooter;

	class TwinFighter : public EnemySpaceship
	{
	public:
		TwinFighter(World* OwningWorld, const string TexturePath = "SpaceShooterRedux/PNG/Enemies/enemyBlack3.png", const sf::Vector2f& Velocity = { 0, 40 });

		virtual void Tick(float DeltaTime) override;

		void OscillateX(float DeltaTime);

	private:
		virtual void Shoot() override;
		unique<BulletShooter> ShooterL;
		unique<BulletShooter> ShooterR;
		float OscTime;
	};
}
#include "Enemy/Boss.h"

namespace we
{
	Boss::Boss(World* World)
		: EnemySpaceship{World, "SpaceShooterRedux/PNG/Enemies/enemyBlack1.png" }
		, Speed{100.f}
		, SwitchDirection{80.f}
	{
		SetActorRotation(sf::degrees(90.f));
		SetSpriteRotationOffset(sf::degrees(-90));
		SetSpriteScale({ 2.3f, 2.3f });
	}

	void Boss::Tick(float DeltaTime)
	{
		EnemySpaceship::Tick(DeltaTime);
	}

	void Boss::Shoot()
	{
	}
}
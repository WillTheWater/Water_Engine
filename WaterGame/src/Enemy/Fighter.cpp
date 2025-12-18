#include "Enemy/Fighter.h"
#include "Weapons/BulletShooter.h"

namespace we
{
	Fighter::Fighter(World* OwningWorld, const string TexturePath, const sf::Vector2f& Velocity)
		: EnemySpaceship{OwningWorld, TexturePath}
		, Shooter{ new BulletShooter{this} }
		, OscTime{0.f}

	{
		SetVelocity(Velocity);
		SetLocalForwardVector(sf::Vector2f{0, 1});
	}

	void Fighter::Tick(float DeltaTime)
	{
		EnemySpaceship::Tick(DeltaTime);
		OscillateX(DeltaTime);
		Shoot();
	}

	void Fighter::OscillateX(float DeltaTime)
	{
		if (IsPendingDestroy())
			return;
		OscTime += DeltaTime;
		float Amplitude = 300.f;
		float Frequency = 0.2f;
		const float XOffset = std::sin(OscTime * 2.f * 3.14159265f * Frequency) * Amplitude;

		AddActorLocationOffset({ XOffset * DeltaTime, 0 });
	}


	void Fighter::Shoot()
	{
		Shooter->Shoot();
	}
}
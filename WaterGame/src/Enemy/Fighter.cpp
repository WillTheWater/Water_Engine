#include "Enemy/Fighter.h"
#include "Weapons/BulletShooter.h"

namespace we
{
	Fighter::Fighter(World* OwningWorld, const string TexturePath, const sf::Vector2f& Velocity)
		: EnemySpaceship{OwningWorld, TexturePath}
		, Shooter{ new BulletShooter{this, 0.5, {0.f, 50.f}} }
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
		static constexpr float Amplitude = 120.f;
		static constexpr float Frequency = 0.5f;

		OscTime += DeltaTime;

		const float XVelocity =
			std::cos(OscTime * 2.f * 3.14159265f * Frequency)
			* Amplitude
			* Frequency
			* 2.f
			* 3.14159265f;

		AddActorLocationOffset({ XVelocity * DeltaTime, 0.f });
	}


	void Fighter::Shoot()
	{
		Shooter->Shoot();
	}
}
#include "Enemy/TwinFighter.h"
#include "Weapons/BulletShooter.h"

namespace we
{
	TwinFighter::TwinFighter(World* OwningWorld, const string TexturePath, const sf::Vector2f& Velocity)
		: EnemySpaceship{OwningWorld, TexturePath}
		, ShooterL{ new BulletShooter{this, 1.f, {-40, 40}, sf::degrees(0)} }
		, ShooterR{ new BulletShooter{this, 1.f, {40, 40}, sf::degrees(0)} }
		, OscTime{0.f}
		, PhaseOffset{}
	{
		SetVelocity(Velocity);
		SetActorRotation(sf::degrees(90.f));
		SetSpriteRotationOffset(sf::degrees(-90));
	}

	void TwinFighter::Tick(float DeltaTime)
	{
		EnemySpaceship::Tick(DeltaTime);
		OscillateX(DeltaTime);
		Shoot();
	}

	void TwinFighter::OscillateX(float DeltaTime)
	{
		static constexpr float Amplitude = 540.f;
		static constexpr float Frequency = 0.15f;

		OscTime += DeltaTime;

		const float XVelocity =
			std::cos(OscTime * 2.f * 3.14159265f * Frequency + PhaseOffset)
			* Amplitude
			* Frequency
			* 2.f
			* 3.14159265f;

		AddActorLocationOffset({ XVelocity * DeltaTime, 0.f });
	}

	void we::TwinFighter::Shoot()
	{
		ShooterL->Shoot();
		ShooterR->Shoot();
	}
}
#include "Spaceship/Spaceship.h"
#include "Framework/MathUtility.h"
#include "VFX/Explosion.h"

namespace we
{
	Spaceship::Spaceship(World* OwningWorld, const string& TexturePath)
		: Actor{OwningWorld, TexturePath}
		, HealthComp{this, 100, 100}
		, Velocity{}
		, BlinkTimer{0.f}
		, BlickDuration{0.2f}
		, BlinkColor{255, 0, 0, 255}
	{

	}

	void Spaceship::BeginPlay()
	{
		Actor::BeginPlay();
		SetPhysicsEnabled(true);
		HealthComp.OnHealthChanged.Bind(GetObject(), &Spaceship::OnHealthChaged);
		HealthComp.OnTakeDamage.Bind(GetObject(), &Spaceship::TakeDamage);
		HealthComp.OnDeath.Bind(GetObject(), &Spaceship::Die);
	}

	void Spaceship::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);
		AddActorLocationOffset(GetVelocity() * DeltaTime);
		UpdateBlink(DeltaTime);
	}

	void Spaceship::Shoot()
	{
	}

	void Spaceship::Damage(float Amount)
	{
		HealthComp.ChangeHealth(-Amount);
	}

	void Spaceship::Blink()
	{
		if (BlinkTimer == 0)
		{
			BlinkTimer = BlickDuration;
		}
	}

	void Spaceship::UpdateBlink(float DeltaTime)
	{
		if (BlinkTimer > 0)
		{
			BlinkTimer -= DeltaTime;
			BlinkTimer = BlinkTimer > 0 ? BlinkTimer : 0.f;
			GetSprite().setColor(LerpColor(sf::Color::White, BlinkColor, BlinkTimer));
		}
	}

	void Spaceship::SetVelocity(sf::Vector2f NewVelocity)
	{
		Velocity = NewVelocity;
	}

	void Spaceship::OnHealthChaged(float Amount, float Health, float MaxHealth)
	{
	}

	void Spaceship::TakeDamage(float Amount, float Health, float MaxHealth)
	{
		Blink();
	}

	void Spaceship::Die()
	{
		Explosion* ExpVFX = new Explosion();
		ExpVFX->SpawnExplosion(GetWorld(), GetActorLocation());
		Destroy();
		delete ExpVFX;
	}
}
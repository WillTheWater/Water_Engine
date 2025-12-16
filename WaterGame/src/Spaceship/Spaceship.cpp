#include "Spaceship/Spaceship.h"

namespace we
{
	Spaceship::Spaceship(World* OwningWorld, const string& TexturePath)
		: Actor{OwningWorld, TexturePath}
		, HealthComp{this, 100, 100}
		, Velocity{}
	{

	}

	void Spaceship::BeginPlay()
	{
		Actor::BeginPlay();
		SetPhysicsEnabled(true);
		HealthComp.OnHealthChanged.Bind(GetObject(), &Spaceship::OnHealthChaged);
		HealthComp.OnTakeDamage.Bind(GetObject(), &Spaceship::Damage);
		HealthComp.OnDeath.Bind(GetObject(), &Spaceship::Die);
	}

	void Spaceship::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);
		AddActorLocationOffset(GetVelocity() * DeltaTime);
	}

	void Spaceship::Shoot()
	{
	}

	void Spaceship::Damage(float Amount)
	{
		HealthComp.ChangeHealth(-Amount);
	}

	void Spaceship::SetVelocity(sf::Vector2f NewVelocity)
	{
		Velocity = NewVelocity;
	}

	void Spaceship::OnHealthChaged(float Amount, float Health, float MaxHealth)
	{
		LOG("Heath: %f", Health)
	}

	void Spaceship::Damage(float Amount, float Health, float MaxHealth)
	{
		LOG("Take %f Damage", Amount)
	}

	void Spaceship::Die()
	{
		LOG("Blow Up!")
		Destroy();
	}
}
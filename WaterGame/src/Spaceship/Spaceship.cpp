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
		HealthComp.OnHealthChanged.Broadcast(15, 90, 100);

	}

	void Spaceship::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);
		AddActorLocationOffset(GetVelocity() * DeltaTime);
	}

	void Spaceship::Shoot()
	{

	}

	void Spaceship::SetVelocity(sf::Vector2f NewVelocity)
	{
		Velocity = NewVelocity;
	}

	void Spaceship::OnHealthChaged(float Amount, float Health, float MaxHealth)
	{
		LOG("Delegat: Amt = %f, Heath = %f/%f", Amount, Health, MaxHealth)
	}
}
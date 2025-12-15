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
}
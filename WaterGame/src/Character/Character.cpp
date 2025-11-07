#include "Character/Character.h"

namespace we
{
	Character::Character(World* OwningWorld, const std::string& TexturePath)
		: Actor{OwningWorld, TexturePath},
		Velocity{}
	{

	}

	void Character::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);
		AddActorLocationOffset(GetVelocity() * DeltaTime);
	}

	void Character::SetVelocity(const sf::Vector2f NewVelocity)
	{
		Velocity = NewVelocity;
	}

	void Character::Shoot()
	{

	}
}

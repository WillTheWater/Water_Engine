#include "Weapons/Bullet.h"

namespace we
{
	Bullet::Bullet(World* OwningWorld, Actor* OwningActor, const string TexturePath, float Velocity, float Damage)
		: Actor{OwningWorld, TexturePath}
		, OwningActor{OwningActor}
		, Velocity{Velocity}
		, Damage{Damage}
	{
	}

	void Bullet::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);
		Move(DeltaTime);
		if (IsOutOfBounds())
		{
			Destroy();
		}
	}

	void Bullet::Move(float DeltaTime)
	{
		AddActorLocationOffset(OwningActor->GetActorFowardVector() * Velocity * DeltaTime);
	}
}
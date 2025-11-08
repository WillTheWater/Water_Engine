#include "Weapons/Projectile.h"

namespace we
{
	Projectile::Projectile(World* OwningWorld, Actor* OwningActor, const std::string& TexturePath, float Velocity, float Damage)
		: Actor{ OwningWorld, TexturePath },
		OwningActor{ OwningActor },
		Velocity{ Velocity },
		Damage{ Damage }
	{
		SetTexture("Bullet/Fire_Bullet_8x5.png", 8, 5, 2.f);
	}
	void Projectile::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);
		Move(DeltaTime);
		if (IsOutOfBounds())
		{
			Destroy();
		}
	}
	void Projectile::Move(float DeltaTime)
	{
		AddActorLocationOffset(GetActorFowardVector() * Velocity * DeltaTime);
	}
	void Projectile::SetSpeed(float NewVelocity)
	{
		Velocity = NewVelocity;
	}
	void Projectile::SetDamage(float NewDamage)
	{
		Damage = NewDamage;
	}
}
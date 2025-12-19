#include "Weapons/Bullet.h"

namespace we
{
	Bullet::Bullet(World* OwningWorld, Actor* OwningActor, const string TexturePath, float Velocity, float Damage)
		: Actor{OwningWorld, TexturePath}
		, OwningActor{OwningActor}
		, Velocity{Velocity}
		, Damage{Damage}
	{
		SetActorID(OwningActor->GetActorID());
		FireDirection = OwningActor->GetActorFowardVector();
	}

	void Bullet::BeginPlay()
	{
		Actor::BeginPlay();
		SetPhysicsEnabled(true);
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

	void Bullet::OnActorBeginOverlap(Actor* OtherActor)
	{
		if (!OtherActor) return;

		if (IsHostile(OtherActor))
		{
			OtherActor->Damage(GetDamage());
			Destroy();
		}
	}

	void Bullet::Move(float DeltaTime)
	{
		AddActorLocationOffset(FireDirection * Velocity * DeltaTime);
	}
}
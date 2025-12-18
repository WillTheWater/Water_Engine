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

		LOG("Bullet Overlap: MyID=%d, OtherID=%d, IsHostile=%d",
			static_cast<int>(GetActorID()),
			static_cast<int>(OtherActor->GetActorID()),
			static_cast<int>(IsHostile(OtherActor)));

		if (IsHostile(OtherActor))
		{
			OtherActor->Damage(GetDamage());
			Destroy();
		}
	}

	void Bullet::Move(float DeltaTime)
	{
		AddActorLocationOffset(OwningActor->GetActorFowardVector() * Velocity * DeltaTime);
	}
}
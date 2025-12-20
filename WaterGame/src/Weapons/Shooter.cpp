#include "Weapons/Shooter.h"
#include "Framework/Actor.h"

namespace we
{
	Shooter::Shooter(Actor* OwningActor)
		: OwningActor{OwningActor}
	{
	}

	void Shooter::Shoot()
	{
		if (!OwningActor || OwningActor->IsPendingDestroy()) { return; }

		if (CanShoot() && !IsOnCooldown())
		{
			ShootImplementation();
		}
	}
}
#include "Weapons/Shooter.h"

namespace we
{
	Shooter::Shooter(Actor* OwningActor)
		: OwningActor{OwningActor}
	{
	}

	void Shooter::Shoot()
	{
		if (CanShoot() && !IsOnCooldown())
		{
			ShootImplimentation();
		}
	}
}
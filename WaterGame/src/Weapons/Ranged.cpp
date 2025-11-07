#include "Weapons/Ranged.h"
#include "Framework/Actor.h"

namespace we
{
	RangedWeapon::RangedWeapon(Actor* OwningActor)
		: OwningActor{ OwningActor }
	{

	}

	void RangedWeapon::Shoot()
	{
		if (CanShoot() && !IsOnCooldown())
		{
			ShootImplimentation();
		}
	}
}

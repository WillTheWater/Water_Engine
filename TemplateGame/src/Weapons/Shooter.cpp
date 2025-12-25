#include "Weapons/Shooter.h"
#include "Framework/Actor.h"

namespace we
{
	Shooter::Shooter(Actor* OwningActor)
		: OwningActor{OwningActor}
		, CurrentWeaponLevel{1}
		, MaxWeaponLevel{4}
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
	
	void Shooter::AddWeaponLevel(int LevelToAdd)
	{
		if (CurrentWeaponLevel == MaxWeaponLevel) { return; }
		CurrentWeaponLevel++;
	}
}
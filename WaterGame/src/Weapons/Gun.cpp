#include "Weapons/Gun.h"
#include "Framework/Core.h"

namespace we
{
	Gun::Gun(Actor* OwningActor, float CooldownTime)
		: RangedWeapon{ OwningActor },
		CooldownTimer{},
		CooldownTime{ CooldownTime }
	{

	}
	bool Gun::IsOnCooldown() const
	{
		if (CooldownTimer.getElapsedTime().asSeconds() > CooldownTime)
		{
			return false;
		}
		return true;
	}
	void Gun::ShootImplimentation()
	{
		CooldownTimer.restart();
		LOG("Shoot")
	}
}
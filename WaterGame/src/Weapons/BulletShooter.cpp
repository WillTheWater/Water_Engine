#include "Weapons/BulletShooter.h"
#include "Framework/Core.h"

namespace we
{
	BulletShooter::BulletShooter(Actor* OwningActor, float CooldownTime)
		: Shooter{OwningActor}
		, CooldownClock{}
		, Cooldown{CooldownTime}
	{
	}

	bool BulletShooter::IsOnCooldown() const
	{
		if (CooldownClock.getElapsedTime().asSeconds() > Cooldown)
		{
			return false;
		}
		return true;
	}

	void BulletShooter::ShootImplimentation()
	{
		CooldownClock.restart();
		LOG("Shoot")

	}
}
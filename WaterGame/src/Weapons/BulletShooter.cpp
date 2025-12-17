#include "Weapons/BulletShooter.h"
#include "Framework/Core.h"
#include "Weapons/Bullet.h"
#include "Framework/World.h"

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
		weak<Bullet> NewBullet = GetOwner()->GetWorld()->SpawnActor<Bullet>(GetOwner(), "SpaceShooterRedux/PNG/Lasers/laserBlue16.png");
		auto BulletPtr = NewBullet.lock();
		if (!BulletPtr)
			return;

		BulletPtr->SetActorLocation(GetOwner()->GetActorLocation());
		BulletPtr->SetActorRotation(GetOwner()->GetActorRotation());
	}
}
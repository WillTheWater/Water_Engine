#include "Weapons/Gun.h"
#include "Framework/Core.h"
#include "Framework/World.h"
#include "Weapons/Projectile.h"

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
		weak<Projectile> Bullet = GetOwner()->GetWorld()->SpawnActor<Projectile>(GetOwner(), "Bullet/Fire_Bullet_8x5.png");
		Bullet.lock()->SetActorLocation(GetOwner()->GetActorLocation());
		Bullet.lock()->SetActorRotation(GetOwner()->GetActorRotation());
	}
}
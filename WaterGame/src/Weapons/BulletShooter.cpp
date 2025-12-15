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
		NewBullet.lock()->SetActorLocation(GetOwner()->GetActorLocation() + sf::Vector2f{0.f, -50.f});
		NewBullet.lock()->SetActorRotation(GetOwner()->GetActorRotation());
	}
}
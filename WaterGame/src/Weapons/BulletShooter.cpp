#include "Weapons/BulletShooter.h"
#include "Framework/Core.h"
#include "Weapons/Bullet.h"
#include "Framework/World.h"

namespace we
{
	BulletShooter::BulletShooter(Actor* OwningActor, float CooldownTime, const sf::Vector2f& ShooterPosOffset, const sf::Angle& ShooterRotOffset)
		: Shooter{ OwningActor }
		, CooldownClock{}
		, Cooldown{ CooldownTime }
		, ShooterPostionOffset{ShooterPosOffset}
		, ShooterRotationOffset{ShooterRotOffset}
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
		sf::Vector2f OwnerForward = GetOwner()->GetActorFowardVector();
		sf::Vector2f OwnerRight = GetOwner()->GetActorRightVector();

		CooldownClock.restart();
		weak<Bullet> NewBullet = GetOwner()->GetWorld()->SpawnActor<Bullet>(GetOwner(), "SpaceShooterRedux/PNG/Lasers/laserBlue16.png");
		auto BulletPtr = NewBullet.lock();
		if (!BulletPtr) { return; }

		BulletPtr->SetActorLocation(GetOwner()->GetActorLocation() + OwnerRight * ShooterPostionOffset.x + OwnerForward * ShooterPostionOffset.y);
		BulletPtr->SetActorRotation(GetOwner()->GetActorRotation() + ShooterRotationOffset);
	}
}
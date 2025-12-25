#include "Weapons/BulletShooter.h"
#include "Framework/Core.h"
#include "Weapons/Bullet.h"
#include "Framework/World.h"

namespace we
{
	BulletShooter::BulletShooter(Actor* OwningActor, float CooldownTime, const sf::Vector2f& ShooterPosOffset, const sf::Angle& ShooterRotOffset, const string& BulletTexturePath)
		: Shooter{ OwningActor }
		, CooldownClock{}
		, Cooldown{ CooldownTime }
		, ShooterPostionOffset{ ShooterPosOffset }
		, ShooterRotationOffset{ ShooterRotOffset }
		, BulletTexturePath{BulletTexturePath}
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

	void BulletShooter::AddWeaponLevel(int LevelToAdd)
	{
		Shooter::AddWeaponLevel(LevelToAdd);
		Cooldown -= 0.1f;
	}

	void BulletShooter::SetBulletTexturePath(const string& NewBulletTexturePath)
	{
		BulletTexturePath = NewBulletTexturePath;
	}

	void BulletShooter::ShootImplementation()
	{
		sf::Vector2f OwnerForward = GetOwner()->GetActorForwardVector();
		sf::Vector2f OwnerRight = GetOwner()->GetActorRightVector();

		CooldownClock.restart();
		weak<Bullet> NewBullet = GetOwner()->GetWorld()->SpawnActor<Bullet>(GetOwner(), BulletTexturePath);
		auto BulletPtr = NewBullet.lock();
		if (!BulletPtr) { return; }

		BulletPtr->SetActorLocation(GetOwner()->GetActorLocation() + OwnerRight * ShooterPostionOffset.x + OwnerForward * ShooterPostionOffset.y);
		BulletPtr->SetActorRotation(GetOwner()->GetActorRotation() + ShooterRotationOffset);
	}
}
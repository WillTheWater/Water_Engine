#pragma once
#include "Weapons/BulletShooter.h"
#include "SFML/Graphics.hpp"

namespace we
{
	class MegaShot : public Shooter
	{
	public:
		MegaShot(Actor* OwningActor, float CooldownTime = 0.8f, const sf::Vector2f& ShooterPosOffset = { 0.f, 0.f }, const sf::Angle& ShooterRotOffset = sf::degrees(0.f));

		BulletShooter ShooterL1;
		BulletShooter ShooterL2;
		BulletShooter ShooterM;
		BulletShooter ShooterR1;
		BulletShooter ShooterR2;
		BulletShooter MaxLvlShooterL;
		BulletShooter MaxLvlShooterR;

		virtual void AddWeaponLevel(int LevelToAdd) override;

	private:
		virtual void ShootImplementation() override;
	};
}
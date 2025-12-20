#pragma once
#include "Weapons/BulletShooter.h"
#include "SFML/Graphics.hpp"

namespace we
{
	class TripleShot : public Shooter
	{
	public:
		TripleShot(Actor* OwningActor, float CooldownTime = 0.8f, const sf::Vector2f& ShooterPosOffset = { 0.f, 0.f }, const sf::Angle& ShooterRotOffset = sf::degrees(0.f));
		
		BulletShooter ShooterL;
		BulletShooter ShooterM;
		BulletShooter ShooterR;
		BulletShooter MaxLvlShooterL;
		BulletShooter MaxLvlShooterR;

		virtual void AddWeaponLevel(int LevelToAdd) override;

	private:
		virtual void ShootImplementation() override;
	};
}
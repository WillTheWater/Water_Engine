#pragma once
#include "SFML/System.hpp"
#include "Weapons/Shooter.h"

namespace we
{
	class BulletShooter : public Shooter
	{
	public:
		BulletShooter(Actor* OwningActor, float CooldownTime = 0.5f, const sf::Vector2f& ShooterPosOffset = {0.f, 0.f}, const sf::Angle& ShooterRotOffset = sf::degrees(0.f));

		virtual bool IsOnCooldown() const override;

	private:
		virtual void ShootImplimentation() override;
		sf::Clock CooldownClock;
		float Cooldown;
		sf::Vector2f ShooterPostionOffset;
		sf::Angle ShooterRotationOffset;
	};
}
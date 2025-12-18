#pragma once
#include "SFML/System.hpp"
#include "Weapons/Shooter.h"

namespace we
{
	class BulletShooter : public Shooter
	{
	public:
		BulletShooter(Actor* OwningActor, float CooldownTime = 0.5f);

		virtual bool IsOnCooldown() const override;

	private:
		virtual void ShootImplimentation() override;
		sf::Clock CooldownClock;
		float Cooldown;
	};
}
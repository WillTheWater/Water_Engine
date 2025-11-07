#pragma once

#include "SFML/System.hpp"
#include "Weapons/Ranged.h"

namespace we
{
	class Gun : public RangedWeapon
	{
	public:
		Gun(Actor* OwningActor, float CooldownTime = 0.8f);

		virtual bool IsOnCooldown() const override;

	private:
		virtual void ShootImplimentation() override;
		sf::Clock CooldownTimer;
		float CooldownTime;
	};
}
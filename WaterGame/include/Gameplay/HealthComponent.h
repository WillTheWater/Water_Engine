#pragma once
#include "Framework/ActorComponent.h"

namespace we
{
	class HealthComponent : public ActorComponent
	{
		HealthComponent(Actor* OwningActor, float Health, float MaxHealth);

		float GetCurrentHealth() const { return CurrentHealth; }
		void SetCurrentHealth(float NewHealth) { CurrentHealth = NewHealth; }
		float GetMaxHealth() const { return MaxHealth; }
		void ChangeHealth(float Amount);

	private:
		void TakeDamage(float Amount);
		void OnDeath();
		void HealthRegen();
		float CurrentHealth;
		float MaxHealth;
	};
}
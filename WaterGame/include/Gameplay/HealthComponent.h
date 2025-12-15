#pragma once
#include "Framework/ActorComponent.h"
#include "Framework/Delegate.h"

namespace we
{
	class HealthComponent : public ActorComponent
	{
	public:
		HealthComponent(Actor* OwningActor, float Health, float MaxHealth);

		float GetCurrentHealth() const { return CurrentHealth; }
		void SetCurrentHealth(float NewHealth) { CurrentHealth = NewHealth; }
		float GetMaxHealth() const { return MaxHealth; }
		void ChangeHealth(float Amount);

		Delegate<float, float, float> OnHealthChanged;

	private:
		void TakeDamage(float Amount);
		void OnDeath();
		void HealthRegen(float Amount);
		float CurrentHealth;
		float MaxHealth;
	};
}
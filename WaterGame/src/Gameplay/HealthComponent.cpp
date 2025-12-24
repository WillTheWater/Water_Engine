#include "Gameplay/HealthComponent.h"
#include "Framework/Core.h"

namespace we
{
	HealthComponent::HealthComponent(Actor* OwningActor, float Health, float MaxHealth)
		: ActorComponent{ OwningActor }
		, CurrentHealth{ Health }
		, MaxHealth{ MaxHealth }
	{

	}

	void HealthComponent::SetInitialHealth(float Health, float NewMaxHealth)
	{
		CurrentHealth = Health;
		MaxHealth = NewMaxHealth;
	}

	float HealthComponent::GetHealthPercentage()
	{
		if (MaxHealth != 0.f)
		{
			return (CurrentHealth / MaxHealth) * 100.f;
		}
	}

	void HealthComponent::ChangeHealth(float Amount)
	{
		if (Amount == 0) return;
		if (CurrentHealth == 0) return;

		CurrentHealth += Amount;
		if (CurrentHealth < 0)
		{
			CurrentHealth = 0;
		}

		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}

		OnHealthChanged.Broadcast(Amount, CurrentHealth, MaxHealth);

		if (Amount < 0)
		{
			TakeDamage(-Amount);
			if (CurrentHealth <= 0)
			{
				Death();
			}
		}
	}

	void HealthComponent::TakeDamage(float Amount)
	{
		OnTakeDamage.Broadcast(Amount, CurrentHealth, MaxHealth);
	}

	void HealthComponent::Death()
	{
		OnDeath.Broadcast();
	}
}
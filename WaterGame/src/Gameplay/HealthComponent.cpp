#include "Gameplay/HealthComponent.h"

namespace we
{
	HealthComponent::HealthComponent(Actor* OwningActor, float Health, float MaxHealth)
		: ActorComponent{ OwningActor }
		, CurrentHealth{Health}
		, MaxHealth{MaxHealth}
	{

	}

	void HealthComponent::ChangeHealth(float Amount)
	{
		if (Amount == 0) { return; }
		if (CurrentHealth == 0) { return; }
		CurrentHealth += Amount;
		if (CurrentHealth <= 0)
		{
			CurrentHealth = 0.f;
		}
		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
		if (Amount < 0)
		{
			TakeDamage(-Amount);
			if (CurrentHealth <= 0)
			{
				CurrentHealth = 0.f;
			}
		}
		else
		{
			HealthRegen();
		}
	}

	void HealthComponent::TakeDamage(float Amount)
	{
	}
}
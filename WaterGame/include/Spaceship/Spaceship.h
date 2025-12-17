#pragma once
#include "Framework/Actor.h"
#include "Gameplay/HealthComponent.h"

namespace we
{
	class Spaceship : public Actor
	{
	public:
		Spaceship(World* OwningWorld, const string& TexturePath = "");

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Shoot();
		virtual void Damage(float Amount) override;

		HealthComponent& GetHealthComponent() { return HealthComp; }
		sf::Vector2f GetVelocity() const { return Velocity; }

		void SetVelocity(sf::Vector2f NewVelocity);

	private:
		void Blink();
		void UpdateBlink(float DeltaTime);
		virtual void OnHealthChaged(float Amount, float Health, float MaxHealth);
		virtual void TakeDamage(float Amount, float Health, float MaxHealth);
		virtual void Die();
		HealthComponent HealthComp;
		sf::Vector2f Velocity;

		float BlinkTimer;
		float BlickDuration;
		sf::Color BlinkColor;
	};
}
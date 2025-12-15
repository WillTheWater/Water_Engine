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

		void SetVelocity(sf::Vector2f NewVelocity);
		sf::Vector2f GetVelocity() const { return Velocity; }

	private:
		void OnHealthChaged(float Amount, float Health, float MaxHealth);
		HealthComponent HealthComp;
		sf::Vector2f Velocity;
	};
}
#pragma once
#include "Spaceship/Spaceship.h"

namespace we
{
	class Shooter;

	class PlayerSpaceship : public Spaceship
	{
	public:
		PlayerSpaceship(World* OwningWorld, const string& TexturePath = "SpaceShooterRedux/PNG/playerShip1_orange.png");

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Shoot() override;
		void SetSpeed(float NewSpeed) { Speed = NewSpeed; }
		virtual void Damage(float Amount) override;

	private:
		void HandleInput();
		void NormalizeInput();
		void ConsumeInput(float DeltaTime);
		void ClampToWindow();
		sf::Vector2f MovementInput;
		float Speed;

		unique<Shooter> PlayerShooter;
	};
}
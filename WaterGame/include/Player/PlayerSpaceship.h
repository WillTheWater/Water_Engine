#pragma once
#include "Spaceship/Spaceship.h"

namespace we
{
	class PlayerSpaceship : public Spaceship
	{
	public:
		PlayerSpaceship(World* OwningWorld, const string& TexturePath = "SpaceShooterRedux/PNG/playerShip1_orange.png");

		virtual void Tick(float DeltaTime) override;
		void SetSpeed(float NewSpeed) { Speed = NewSpeed; }
	private:
		void HandleInput();
		void ConsumeInput(float DeltaTime);
		sf::Vector2f MovementInput;
		float Speed;
	};
}
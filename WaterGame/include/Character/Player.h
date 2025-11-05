#pragma once

#include "Character/Character.h"

namespace we
{
	class PlayerCharacter : public Character
	{
	public:
		PlayerCharacter(World* OwningWorld, const std::string& TexturePath = "character/idle/idle_down_40x40.png");

		virtual void Tick(float DeltaTime) override;

		void SetMovementSpeed(float Speed) { MovementSpeed = Speed; }
		float GetSpeed() { return MovementSpeed; }

	private:
		void HandleInput();
		void ConsumeIput(float DeltaTime);
		sf::Vector2f MoveInput;
		float MovementSpeed;
	};
}
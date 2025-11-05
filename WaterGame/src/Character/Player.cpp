#include "Character/Player.h"

namespace we
{
	PlayerCharacter::PlayerCharacter(World* OwningWorld, const std::string& TexturePath)
		: Character{ OwningWorld, TexturePath},
		MoveInput{},
		MovementSpeed{ 200.f }
	{
		SetTexture(TexturePath, 40, 40, 4);
	}
	void PlayerCharacter::Tick(float DeltaTime)
	{
		Character::Tick(DeltaTime);
		HandleInput();
		ConsumeIput(DeltaTime);
	}
	void PlayerCharacter::HandleInput()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			MoveInput.y = -1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			MoveInput.x = -1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			MoveInput.y = 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			MoveInput.x = 1.f;
		}
	}
	void PlayerCharacter::ConsumeIput(float DeltaTime)
	{
		SetVelocity(MoveInput * MovementSpeed);
		MoveInput.x = MoveInput.y = 0.f;
	}
}

#include "Character/Player.h"
#include "Framework/MathUtility.h"
#include "Weapons/Gun.h"

namespace we
{
	PlayerCharacter::PlayerCharacter(World* OwningWorld, const std::string& TexturePath)
		: Character{ OwningWorld, TexturePath},
		MoveInput{},
		MovementSpeed{ 600.f },
		Handgun{ new Gun{this}}
	{
		SetTexture(TexturePath, 40, 40, 4);
	}
	void PlayerCharacter::Tick(float DeltaTime)
	{
		Character::Tick(DeltaTime);
		HandleInput();
		ConsumeIput(DeltaTime);
	}
	void PlayerCharacter::Shoot()
	{
		if (Handgun)
		{
			Handgun->Shoot();
		}
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
		SetWindowBoundery();
		NomalizeInput();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		{
			Shoot();
		}
	}
	void PlayerCharacter::ConsumeIput(float DeltaTime)
	{
		SetVelocity(MoveInput * MovementSpeed);
		MoveInput.x = MoveInput.y = 0.f;
	}
	void PlayerCharacter::SetWindowBoundery()
	{
		sf::FloatRect SpriteBounds = GetSpriteBounds();
		sf::Vector2u WindowSize = GetWindowSize();
		sf::Vector2f WindowSizef(static_cast<float>(WindowSize.x),
			static_cast<float>(WindowSize.y));
		
		// Prevent horizontal movement out of bounds
		if (SpriteBounds.position.x <= 0.f && MoveInput.x < 0.f)
		{
			MoveInput.x = 0.f;
		}
		else if (SpriteBounds.position.x + SpriteBounds.size.x >= WindowSizef.x && MoveInput.x > 0.f)
		{
			MoveInput.x = 0.f;
		}

		// Prevent vertical movement out of bounds
		if (SpriteBounds.position.y <= 0.f && MoveInput.y < 0.f)
		{
			MoveInput.y = 0.f;
		}
		else if (SpriteBounds.position.y + SpriteBounds.size.y >= WindowSizef.y && MoveInput.y > 0.f)
		{
			MoveInput.y = 0.f;
		}
	}
	void PlayerCharacter::NomalizeInput()
	{
		Normalize(MoveInput);
	}
}

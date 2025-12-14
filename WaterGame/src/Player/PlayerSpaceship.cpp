#include "Player/PlayerSpaceship.h"
#include "SFML/System.hpp"
#include "Framework/MathUtility.h"

namespace we
{
	PlayerSpaceship::PlayerSpaceship(World* OwningWorld, const string& TexturePath)
		: Spaceship{OwningWorld, TexturePath}
		, MovementInput{}
		, Speed{700.f}
	{
	}

	void PlayerSpaceship::Tick(float DeltaTime)
	{
		Spaceship::Tick(DeltaTime);
		HandleInput();
		ConsumeInput(DeltaTime);
	}

	void PlayerSpaceship::HandleInput()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			MovementInput.y = -1.f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			MovementInput.y = 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			MovementInput.x = -1.f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			MovementInput.x = 1.f;
		}
		NormalizeInput();
	}

	void PlayerSpaceship::NormalizeInput()
	{
		Normalize(MovementInput);
	}

	void PlayerSpaceship::ConsumeInput(float DeltaTime)
	{
		SetVelocity(MovementInput * Speed);
		MovementInput.x = MovementInput.y = 0.f;
	}
}
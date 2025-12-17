#include "Player/PlayerSpaceship.h"
#include "SFML/System.hpp"
#include "Framework/MathUtility.h"
#include "Weapons/BulletShooter.h"

namespace we
{
	PlayerSpaceship::PlayerSpaceship(World* OwningWorld, const string& TexturePath)
		: Spaceship{OwningWorld, TexturePath}
		, MovementInput{}
		, Speed{700.f}
		, PlayerShooter{ new BulletShooter{this} }
	{
		SetActorID(EActorID::Player);
	}

	void PlayerSpaceship::BeginPlay()
	{
		Actor::BeginPlay();
		SetLocalForwardVector(sf::Vector2f{ 0,-1 });
	}

	void PlayerSpaceship::Tick(float DeltaTime)
	{
		Spaceship::Tick(DeltaTime);
		HandleInput();
		ConsumeInput(DeltaTime);
	}

	void PlayerSpaceship::Shoot()
	{
		if (PlayerShooter)
		{
			PlayerShooter->Shoot();
		}
	}

	void PlayerSpaceship::Damage(float Amount)
	{
		Spaceship::Damage(Amount);
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
		ClampToWindow();
		NormalizeInput();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			Shoot();
		}
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

	void PlayerSpaceship::ClampToWindow()
	{
		const sf::Vector2u windowSize = GetWindowSize();
		sf::Vector2f position = GetActorLocation();

		const sf::FloatRect bounds = GetSprite().getGlobalBounds();

		const float halfWidth = bounds.size.x * 0.5f;
		const float halfHeight = bounds.size.y * 0.5f;

		position.x = clamp(position.x, halfWidth, windowSize.x - halfWidth);
		position.y = clamp(position.y, halfHeight, windowSize.y - halfHeight);

		SetActorLocation(position);
	}
}
#include "Player/PlayerSpaceship.h"
#include "SFML/System.hpp"
#include "Framework/MathUtility.h"
#include "Weapons/BulletShooter.h"
#include "Weapons/TripleShot.h"
#include "Weapons/MegaShot.h"

namespace we
{
	PlayerSpaceship::PlayerSpaceship(World* OwningWorld, const string& TexturePath)
		: Spaceship{OwningWorld, TexturePath}
		, MovementInput{}
		, Speed{700.f}
		, PlayerShooter{ new BulletShooter{this, 0.3f, {0.f, 35.f}, sf::degrees(0)}}
		, InvulnerabilityTime{1.5f}
		, InvulnerabilityHandle{}
		, bIsInvulnerable{true}
		, InvulnerabilityBlinkInterval{0.2f}
		, InvulnerabilityBlinkTimer{0.f}
		, InvulnerabilityBlink{1.f}
	{
		SetActorID(EActorID::Player);
	}

	void PlayerSpaceship::BeginPlay()
	{
		Spaceship::BeginPlay();
		SetActorRotation(sf::degrees(-90));
		SetSpriteRotationOffset(sf::degrees(90));
		SetPhysicsEnabled(false);
		TimerManager::Get().SetTimer(GetObject(), &PlayerSpaceship::ResetInvulnerability, InvulnerabilityTime);
	}

	void PlayerSpaceship::Tick(float DeltaTime)
	{
		Spaceship::Tick(DeltaTime);
		HandleInput();
		ConsumeInput(DeltaTime);
		BlinkInvulnerability(DeltaTime);
	}

	void PlayerSpaceship::Shoot()
	{
		if (PlayerShooter && !bIsInvulnerable)
		{
			PlayerShooter->Shoot();
		}
	}

	void PlayerSpaceship::Damage(float Amount)
	{
		if (!bIsInvulnerable)
		{
			Spaceship::Damage(Amount);
		}
	}

	void PlayerSpaceship::SetWeapon(unique<Shooter>&& NewWeapon)
	{
		if (PlayerShooter && typeid(*PlayerShooter.get()) == typeid(*NewWeapon.get()))
		{
			PlayerShooter->AddWeaponLevel();
			return;
		}
		PlayerShooter = std::move(NewWeapon);
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

	void PlayerSpaceship::ResetInvulnerability()
	{
		GetSprite().setColor(sf::Color::White);
		SetPhysicsEnabled(true);
		bIsInvulnerable = false;
	}

	void PlayerSpaceship::BlinkInvulnerability(float DeltaTime)
	{
		if (!bIsInvulnerable) { return; }
		InvulnerabilityBlinkTimer += DeltaTime * InvulnerabilityBlink;
		if (InvulnerabilityBlinkTimer < 0 || InvulnerabilityBlinkTimer > InvulnerabilityBlinkInterval)
		{
			InvulnerabilityBlink *= -1;
		}
		GetSprite().setColor(LerpColor({ 255,255,255,40 }, { 255,255,255,180 }, InvulnerabilityBlinkTimer / InvulnerabilityBlinkInterval));
	}
}
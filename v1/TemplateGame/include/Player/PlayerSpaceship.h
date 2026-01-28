#pragma once
#include "Spaceship/Spaceship.h"
#include "Framework/TimerManager.h"

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
		virtual void Damage(float Amount) override;
		void SetSpeed(float NewSpeed) { Speed = NewSpeed; }
		void SetWeapon(unique<Shooter>&& NewWeapon);

	private:
		void HandleInput();
		void NormalizeInput();
		void ConsumeInput(float DeltaTime);
		void ClampToWindow();
		void ResetInvulnerability();
		void BlinkInvulnerability(float DeltaTime);
		sf::Vector2f MovementInput;
		float Speed;

		unique<Shooter> PlayerShooter;
		float InvulnerabilityTime;
		TimerHandle InvulnerabilityHandle;
		bool bIsInvulnerable;
		float InvulnerabilityBlinkInterval;
		float InvulnerabilityBlinkTimer;
		float InvulnerabilityBlink;
	};
}
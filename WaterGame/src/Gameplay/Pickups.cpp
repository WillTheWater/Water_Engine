#include "Gameplay/Pickups.h"
#include "Player/PlayerSpaceship.h"
#include "Weapons/TripleShot.h"
#include "Weapons/MegaShot.h"
#include "Framework/World.h"
#include "GameFramework/PlayerManager.h"

namespace we
{
	Pickup::Pickup(World* World, const string& TexturePath, PickupInteractFunction PickupFunction, float Speed)
		: Actor{World, TexturePath}
		, PickupFunction{PickupFunction}
		, Speed{Speed}
	{
	}

	void Pickup::BeginPlay()
	{
		Actor::BeginPlay();
		SetPhysicsEnabled(true);
	}

	void Pickup::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);
		Move(DeltaTime);
	}

	void Pickup::OnActorBeginOverlap(Actor* OtherActor)
	{
		if (!OtherActor || OtherActor->IsPendingDestroy()) { return; }
		if (!PlayerManager::Get().GetPlayer()) { return; }
		weak<PlayerSpaceship> Player = PlayerManager::Get().GetPlayer()->GetPlayerSpaceship();
		if (Player.expired() || Player.lock()->IsPendingDestroy()) { return; }
		if (Player.lock().get() == OtherActor)
		{
			PickupFunction(Player.lock().get());
			Destroy();
		}
	}

	void Pickup::Move(float DeltaTime)
	{
		AddActorLocationOffset({ 0.f, Speed * DeltaTime });
	}

	void HealthPickup(PlayerSpaceship* Player)
	{
		static float HealFor = 20.f;
		if (Player && !Player->IsPendingDestroy())
		{
			Player->GetHealthComponent().ChangeHealth(HealFor);
		}
	}

	void TripleShotPickup(PlayerSpaceship* Player)
	{
		if (Player && !Player->IsPendingDestroy())
		{
			Player->SetWeapon(unique<Shooter>{new TripleShot{ Player, 0.1f, {0.f, 35.f} }});
		}
	}

	void MegaShotPickup(PlayerSpaceship* Player)
	{
		if (Player && !Player->IsPendingDestroy())
		{
			Player->SetWeapon(unique<Shooter>{new MegaShot{ Player, 0.1f, {0.f, 35.f} }});
		}
	}

	weak<Pickup> SpawnPickup(World* World, const string& TexturePath, PickupInteractFunction PickupFunction)
	{
		weak<Pickup> NewPickup = World->SpawnActor<Pickup>(TexturePath, PickupFunction);
		return NewPickup;
	}

	weak<Pickup> SpawnHealthPickup(World* World)
	{
		return SpawnPickup(World, "SpaceShooterRedux/PNG/Power-ups/pill_red.png", HealthPickup);
	}

	weak<Pickup> SpawnTripleShotPickup(World* World)
	{
		return SpawnPickup(World, "SpaceShooterRedux/PNG/Power-ups/bold_silver.png", TripleShotPickup);
	}

	weak<Pickup> SpawnMegaShotPickup(World* World)
	{
		return SpawnPickup(World, "SpaceShooterRedux/PNG/Power-ups/bolt_gold.png", MegaShotPickup);
	}
}
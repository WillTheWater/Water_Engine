#include "Gameplay/Pickups.h"
#include "Player/PlayerSpaceship.h"

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
	}

	void Pickup::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);
		Move(DeltaTime);
	}

	void Pickup::OnActorBeginOverlap(Actor* OtherActor)
	{
		// TODO: Do better than casting
		PlayerSpaceship* Player = dynamic_cast<PlayerSpaceship*>(OtherActor);
		if (Player != nullptr && !Player->IsPendingDestroy())
		{
			PickupFunction(Player);
		}
	}

	void Pickup::Move(float DeltaTime)
	{
		AddActorLocationOffset({ 0.f, Speed * DeltaTime });
	}
}
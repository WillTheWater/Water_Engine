#pragma once
#include "Framework/Actor.h"
#include "Framework/Core.h"

namespace we
{
	class PlayerSpaceship;
	class Pickup;
	using PickupInteractFunction = std::function<void(PlayerSpaceship*)>;
	using PickupFactoryFunction = std::function<weak<Pickup>(World*)>;

	class Pickup : public Actor
	{
	public:
		Pickup(World* World, const string& TexturePath, PickupInteractFunction PickupFunction, float Speed = 200.f);

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void OnActorBeginOverlap(Actor* OtherActor) override;

	private:
		void Move(float DeltaTime);
		PickupInteractFunction PickupFunction;
		float Speed;
	};

	void HealthPickup(PlayerSpaceship* Player);
	void TripleShotPickup(PlayerSpaceship* Player);
	void MegaShotPickup(PlayerSpaceship* Player);
	void ExtraLifePickup(PlayerSpaceship* Player);

	weak<Pickup> SpawnPickup(World* World, const string& TexturePath, PickupInteractFunction PickupFunction);
	weak<Pickup> SpawnHealthPickup(World* World);
	weak<Pickup> SpawnTripleShotPickup(World* World);
	weak<Pickup> SpawnMegaShotPickup(World* World);
	weak<Pickup> SpawnExtraLifePickup(World* World);
}
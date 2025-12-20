#pragma once
#include "Framework/Actor.h"

namespace we
{
	class PlayerSpaceship;
	using PickupInteractFunction = std::function<void(PlayerSpaceship*)>;

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
}
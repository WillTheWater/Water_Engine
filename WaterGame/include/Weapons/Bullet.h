#pragma once
#include "Framework/Actor.h"

namespace we
{
	class Bullet : public Actor
	{
	public:
		Bullet(World* OwningWorld, Actor* OwningActor, const string TexturePath, float Velocity = 1000.f, float Damage = 10.f);

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		void SetVelocity(float NewVelocity) { Velocity = NewVelocity; }
		void SetDamage(float NewDamage) { Damage = NewDamage; }
		float GetDamage() const { return Damage; }

		virtual void OnActorBeginOverlap(Actor* OtherActor) override;
	private:

		void Move(float DeltaTime);
		Actor* OwningActor;
		float Velocity;
		float Damage;
	};
}
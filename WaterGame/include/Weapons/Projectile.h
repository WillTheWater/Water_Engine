#pragma once

#include "Framework/Actor.h"

namespace we
{
	class Projectile : public Actor
	{
	public:
		Projectile(World* OwningWorld, Actor* OwningActor, const std::string& TexturePath, float Velocity = 1800.f , float Damage = 10.f);

		virtual void Tick(float DeltaTime) override;

		void Move(float DeltaTime);
		void SetSpeed(float NewVelocity);
		void SetDamage(float NewDamage);

	private:
		Actor* OwningActor;
		float Velocity;
		float Damage;
	};
}
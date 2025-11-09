#pragma once

#include "Framework/Actor.h"

namespace we
{
	class Character : public Actor
	{
	public:
		Character(World* OwningWorld, const std::string& TexturePath = "");
		virtual void Tick(float DeltaTime) override;

		void SetVelocity(const sf::Vector2f NewVelocity);
		sf::Vector2f GetVelocity() const { return Velocity; }
		virtual void Shoot();
		virtual void BeginPlay() override;

	private:
		sf::Vector2f Velocity;
	};
}
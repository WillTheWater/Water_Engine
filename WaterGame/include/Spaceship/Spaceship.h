#pragma once
#include "Framework/Actor.h"

namespace we
{
	class Spaceship : public Actor
	{
	public:
		Spaceship(World* OwningWorld, const string& TexturePath = "");

		virtual void Tick(float DeltaTime) override;

		void SetVelocity(sf::Vector2f NewVelocity);
		sf::Vector2f GetVelocity() const { return Velocity; }

	private:
		sf::Vector2f Velocity;
	};
}
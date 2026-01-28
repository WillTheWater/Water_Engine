#pragma once
#include "Framework/Actor.h"

namespace we
{
	class Partical : public Actor
	{
	public:
		Partical(World* OwningWorld, const string& TexturePath = "");

		virtual void Tick(float DeltaTime) override;

		void HandleLifetime();

		void RandomVelocity(float Min, float Max);
		void RandomSize(float Min, float Max);
		void RandomLifetime(float Min, float Max);

	private:
		void Move(float DeltaTime);
		void Fade(float DeltaTime);
		sf::Vector2f Velocity;
		float Lifetime;
		sf::Clock Timer;

	};
}
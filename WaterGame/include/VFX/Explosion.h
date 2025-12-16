#pragma once
#include "Framework/Core.h"
#include "SFML/Graphics.hpp"

namespace we
{
	class World;

	class Explosion
	{
	public:
		Explosion(int ParticalAmount = 20, float MinLife = 0.2f, float MaxLife = 1.f, float MinSize = 0.3f, float MaxSize = 2.f, float MinSpeed = 200.f, float MaxSpeed = 800.f, const sf::Color& ParticalColor = sf::Color::White, const List<string>& ParticalImagePaths = {
			"SpaceShooterRedux/PNG/Effects/star1.png",
			"SpaceShooterRedux/PNG/Effects/star2.png",
			"SpaceShooterRedux/PNG/Effects/star3.png" });

		void SpawnExplosion(World* World, const sf::Vector2f& SpawnLocation);

	private:
		int ParticalCount;
		float LifetimeMin;
		float LifetimeMax;
		float SizeMin;
		float SizeMax;
		float SpeedMin;
		float SpeedMax;
		sf::Color ParticalColor;
		List<string> ParticalImagePaths;
	};
}
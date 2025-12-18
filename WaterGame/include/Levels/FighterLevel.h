#pragma once
#include "GameMode/Level.h"
#include "SFML/Graphics.hpp"
#include "Framework/TimerManager.h"

namespace we
{
	class FighterLevel : public Level
	{
	public:
		FighterLevel(World* World);

		virtual void BeginLevel() override;

	private:
		virtual void EndLevel() override;
		void SpawnFighter();
		void SwitchRow();
		float SpawnInterval;
		float SwitchInterval;
		float SpawnDistance;
		sf::Vector2f SpawnLocationOne;
		sf::Vector2f SpawnLocationTwo;
		sf::Vector2f SpawnLocation;

		TimerHandle SpawnTimerHandle;
		TimerHandle SwitchTimerHandle;

		int RowsToSpawn;
		int RowsSpawned;
		int FightersPerRow;
		int CurrentFighterCount;

	};
}
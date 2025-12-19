#pragma once
#include "GameMode/Level.h"
#include "SFML/Graphics.hpp"
#include "Framework/TimerManager.h"

namespace we
{
	class TwinFighterLevel : public Level
	{
	public:
		TwinFighterLevel(World* World);

		virtual void BeginLevel() override;

	private:
		virtual void EndLevel() override;
		void SpawnTwinFighter();
		void SwitchRow();
		float SpawnInterval;
		float SpawnDistance;
		sf::Vector2f SpawnLocationL;
		sf::Vector2f SpawnLocationR;
		sf::Vector2f SpawnLocation;
		int TwinFightersToSpawn;
		int CurrentTwinFighterCount;
		bool bSpawnL;

		TimerHandle SpawnTimerHandle;
	};
}
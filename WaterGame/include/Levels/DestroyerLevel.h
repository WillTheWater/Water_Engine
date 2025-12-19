#pragma once
#include "GameMode/Level.h"
#include "SFML/Graphics.hpp"
#include "Framework/TimerManager.h"

namespace we
{
	class DestroyerLevel : public Level
	{
	public:
		DestroyerLevel(World* World);

		virtual void BeginLevel() override;

	private:
		virtual void EndLevel() override;
		void SpawnDestroyer();
		void SwitchRow();
		float SpawnInterval;
		sf::Vector2f SpawnLocationL;
		sf::Vector2f SpawnLocationR;
		sf::Vector2f SpawnLocation;
		int DestroyerToSpawn;
		int CurrentDestroyerCount;
		bool bSpawnL;

		TimerHandle SpawnTimerHandle;
	};
}
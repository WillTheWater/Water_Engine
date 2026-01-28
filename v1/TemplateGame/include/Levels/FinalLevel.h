#pragma once
#include "GameMode/Level.h"
#include "Framework/TimerManager.h"

namespace we
{
	class FinalLevel :public Level
	{
	public:
		FinalLevel(World* World);

		virtual void BeginLevel() override;
		virtual void TickLevel(float DeltaTime) override; 

	private:
		virtual void EndLevel() override;
		void SpawnFighter();
		void SpawnTwinFighter();
		void SpawnDestroyer();
		void IncreaseDifficulty();
		void LevelDurationDurationReached();
		sf::Vector2f GetRandomSpawnLocation() const;
		float SpawnInterval;
		float MinSpawnInterval;
		float SpawnIntervalDecrement;
		float SpawnDifficultyInterval;
		float LevelDuration;
		TimerHandle DifficultyHandle;
		TimerHandle SpawnHandle;
	};
}
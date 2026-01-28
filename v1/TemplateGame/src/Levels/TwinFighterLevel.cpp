#include "Framework/World.h"
#include "Levels/TwinFighterLevel.h"
#include "Enemy/TwinFighter.h"

namespace we
{
	TwinFighterLevel::TwinFighterLevel(World* World)
		: Level{ World }
		, SpawnInterval{ 5.f }
		, SpawnDistance{ 300.f }
		, SpawnLocationL{World->GetWindowSize().x/2 - SpawnDistance, -60.f}
		, SpawnLocationR{World->GetWindowSize().x/2 + SpawnDistance, -60.f }
		, SpawnLocation{ SpawnLocationL }
		, TwinFightersToSpawn{ 3 }
		, CurrentTwinFighterCount{ 0 }
		,bSpawnL{true}
	{
	}

	void TwinFighterLevel::BeginLevel()
	{
		SpawnTimerHandle = TimerManager::Get().SetTimer(GetObject(), &TwinFighterLevel::SpawnTwinFighter, SpawnInterval, true);
	}

	void TwinFighterLevel::EndLevel()
	{
		TimerManager::Get().ClearTimer(SpawnTimerHandle);
		LOG("Level Ended")
	}

	void TwinFighterLevel::SpawnTwinFighter()
	{
		weak<TwinFighter> NewFighter = GetWorld()->SpawnActor<TwinFighter>();
		auto FighterSpawned = NewFighter.lock();
		if (!FighterSpawned) return;
		FighterSpawned->PhaseOffset = (CurrentTwinFighterCount%2 == 0 ? 0.f : 3.14159265f);
		sf::Vector2f CurrentSpawn = bSpawnL ? SpawnLocationL : SpawnLocationR;
		FighterSpawned->SetActorLocation(CurrentSpawn);

		bSpawnL = !bSpawnL;

		CurrentTwinFighterCount++;
		if (CurrentTwinFighterCount == TwinFightersToSpawn)
		{
			LevelEnd();
		}
	}
}
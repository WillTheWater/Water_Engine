#include "Levels/FinalLevel.h"

namespace we
{
	FinalLevel::FinalLevel(World* World)
		: Level{World}
		, SpawnInterval{4.f}
		, MinSpawnInterval{0.6f}
		, SpawnIntervalDecrement{0.4f}
		, SpawnDifficultyInterval{8.f}
		, LevelDuration{60.f}
		, DifficultyHandle{}
		, SpawnHandle{}
	{
	}

	void FinalLevel::BeginLevel()
	{
		SpawnHandle = TimerManager::Get().SetTimer(GetObject(), &FinalLevel::SpawnFighter, SpawnInterval);
		DifficultyHandle = TimerManager::Get().SetTimer(GetObject(), &FinalLevel::IncreaseDifficulty, SpawnIntervalDecrement);
		TimerManager::Get().SetTimer(GetObject(), &FinalLevel::LevelDurationDurationReached, LevelDuration);
	}

	void FinalLevel::TickLevel(float DeltaTime)
	{
	}

	void FinalLevel::EndLevel()
	{
	}
	void FinalLevel::SpawnFighter()
	{
	}
	void FinalLevel::SpawnTwinFighter()
	{
	}
	void FinalLevel::SpawnDestroyer()
	{
	}
	void FinalLevel::IncreaseDifficulty()
	{
	}
	void FinalLevel::LevelDurationDurationReached()
	{
	}
	sf::Vector2f FinalLevel::GetRandomSpawnLocation() const
	{
	}
}
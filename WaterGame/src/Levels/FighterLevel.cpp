#include "Framework/World.h"
#include "Levels/FighterLevel.h"
#include "Enemy/Fighter.h"

namespace we
{
	FighterLevel::FighterLevel(World* World)
		: Level{World}
		, SpawnInterval{3.5f}
		, SwitchInterval{4.f}
		, SpawnDistance{100.f}
		, SpawnLocationOne{0.f, 0.f}
		, SpawnLocationTwo{ 0.f, 0.f }
		, SpawnLocation{ 0.f, 0.f }
		, RowsToSpawn{2}
		, RowsSpawned{0}
		, FightersPerRow{3}
		, CurrentFighterCount{0}
	{
	}

	void FighterLevel::BeginLevel()
	{
		auto WindowSize = GetWorld()->GetWindowSize();

		const float ySpawn = -40.f;
		SpawnLocationOne = sf::Vector2f{ WindowSize.x * (1.f / 3.f), ySpawn };
		SpawnLocationTwo = sf::Vector2f{ WindowSize.x * (2.f / 3.f), ySpawn };

		SpawnLocation = SpawnLocationOne;

		SwitchRow();
	}

	void FighterLevel::EndLevel()
	{
		TimerManager::Get().ClearTimer(SpawnTimerHandle);
		TimerManager::Get().ClearTimer(SwitchTimerHandle);
		LOG("Level Ended")
	}

	void FighterLevel::SpawnFighter()
	{
		weak<Fighter> NewFighter = GetWorld()->SpawnActor<Fighter>();
		auto FighterSpawned = NewFighter.lock();
		if (FighterSpawned)
		{
			FighterSpawned->SetActorLocation(SpawnLocation);
			CurrentFighterCount++;
			if (CurrentFighterCount == FightersPerRow)
			{
				TimerManager::Get().ClearTimer(SpawnTimerHandle);
				SwitchTimerHandle = TimerManager::Get().SetTimer(GetObject(), &FighterLevel::SwitchRow, SwitchInterval, false);
			}
		}
	}

	void FighterLevel::SwitchRow()
	{
		if (RowsSpawned >= RowsToSpawn)
		{
			LevelEnd();
			return;
		}

		CurrentFighterCount = 0;

		SpawnLocation =
			(SpawnLocation == SpawnLocationOne)
			? SpawnLocationTwo
			: SpawnLocationOne;

		SpawnTimerHandle =
			TimerManager::Get().SetTimer(
				GetObject(),
				&FighterLevel::SpawnFighter,
				SpawnInterval,
				true
			);

		RowsSpawned++;
	}

}
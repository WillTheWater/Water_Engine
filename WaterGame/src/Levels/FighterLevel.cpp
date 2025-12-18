#include "Framework/World.h"
#include "Levels/FighterLevel.h"
#include "Enemy/Fighter.h"

namespace we
{
	FighterLevel::FighterLevel(World* World)
		: Level{World}
		, SpawnInterval{2.f}
		, SwitchInterval{6.f}
		, SpawnDistance{100.f}
		, SpawnLocationOne{0.f, 0.f}
		, SpawnLocationTwo{ 0.f, 0.f }
		, SpawnLocation{ 0.f, 0.f }
		, RowsToSpawn{4}
		, RowsSpawned{0}
		, FightersPerRow{5}
		, CurrentFighterCount{0}
	{
	}

	void FighterLevel::BeginLevel()
	{
		auto WindowSize = GetWorld()->GetWindowSize();
		SpawnLocationOne = sf::Vector2f{ SpawnDistance, -100.f };
		SpawnLocationOne = sf::Vector2f{ WindowSize.x - SpawnDistance, -100.f };

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
		if (RowsSpawned == RowsToSpawn)
		{
			EndLevel();
			return;
		}

		if (SpawnLocation == SpawnLocationOne)
		{
			SpawnLocation = SpawnLocationTwo;
		}
		else
		{
			SpawnLocation = SpawnLocationOne;
		}
		SpawnTimerHandle = TimerManager::Get().SetTimer(GetObject(), &FighterLevel::SpawnFighter, SpawnInterval, true);
		RowsSpawned++;
	}
}
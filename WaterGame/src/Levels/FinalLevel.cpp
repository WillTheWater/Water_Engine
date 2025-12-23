#include "Levels/FinalLevel.h"
#include "Enemy/Fighter.h"
#include "Enemy/TwinFighter.h"
#include "Enemy/Destroyer.h"
#include "Framework/World.h"
#include "Framework/MathUtility.h"

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
		TimerManager::Get().ClearTimer(SpawnHandle);
		TimerManager::Get().ClearTimer(DifficultyHandle);
	}

	void FinalLevel::SpawnFighter()
	{
		weak<Fighter> NewFighter = GetWorld()->SpawnActor<Fighter>();
		NewFighter.lock()->SetActorLocation(GetRandomSpawnLocation());
		SpawnHandle = TimerManager::Get().SetTimer(GetObject(), &FinalLevel::SpawnTwinFighter, SpawnInterval);
	}

	void FinalLevel::SpawnTwinFighter()
	{
		weak<TwinFighter> NewTwinFighter = GetWorld()->SpawnActor<TwinFighter>();
		NewTwinFighter.lock()->SetActorLocation(GetRandomSpawnLocation());
		SpawnHandle = TimerManager::Get().SetTimer(GetObject(), &FinalLevel::SpawnDestroyer, SpawnInterval);
	}

	void FinalLevel::SpawnDestroyer()
	{
		weak<Destroyer> NewDestroyer = GetWorld()->SpawnActor<Destroyer>();
		NewDestroyer.lock()->SetActorLocation(GetRandomSpawnLocation());
		SpawnHandle = TimerManager::Get().SetTimer(GetObject(), &FinalLevel::SpawnFighter, SpawnInterval);
	}

	void FinalLevel::IncreaseDifficulty()
	{
		SpawnInterval -= SpawnIntervalDecrement;
		if (SpawnInterval < MinSpawnInterval)
		{
			SpawnInterval = MinSpawnInterval;
		}
	}

	void FinalLevel::LevelDurationDurationReached()
	{
		LevelEnd();
	}

	sf::Vector2f FinalLevel::GetRandomSpawnLocation() const
	{
		auto WindowSize = GetWorld()->GetWindowSize();
		float SpawnX = RandomRange(100, WindowSize.x - 100.f);
		float SpawnY = -80.f;
		return { SpawnX, SpawnY };
	}
}
#include "Framework/World.h"
#include "Levels/DestroyerLevel.h"
#include "Enemy/Destroyer.h"

namespace we
{
	DestroyerLevel::DestroyerLevel(World* World)
		: Level{ World }
		, SpawnInterval{ 5.f }
		, SpawnLocationL{ World->GetWindowSize().x * (1.f / 3.f), -40.f }
		, SpawnLocationR{ World->GetWindowSize().x * (2.f / 3.f), -40.f }
		, SpawnLocation{ SpawnLocationL }
		, DestroyerToSpawn{ 10 }
		, CurrentDestroyerCount{ 0 }
		, bSpawnL{ true }
	{
	}

	void DestroyerLevel::BeginLevel()
	{
		SpawnDestroyer();
		SpawnTimerHandle = TimerManager::Get().SetTimer(GetObject(), &DestroyerLevel::SpawnDestroyer, SpawnInterval, true);
	}

	void DestroyerLevel::EndLevel()
	{
		TimerManager::Get().ClearTimer(SpawnTimerHandle);
		LOG("Level Ended")
	}

	void DestroyerLevel::SpawnDestroyer()
	{
		if (CurrentDestroyerCount >= DestroyerToSpawn)
		{
			LevelEnd();
			return;
		}

		weak<Destroyer> NewDestroyer = GetWorld()->SpawnActor<Destroyer>();
		auto DestroyerSpawned = NewDestroyer.lock();
		if (!DestroyerSpawned) return;

		const sf::Vector2f SpawnPos = bSpawnL ? SpawnLocationL : SpawnLocationR;
		DestroyerSpawned->SetActorLocation(SpawnPos);

		bSpawnL = !bSpawnL;

		CurrentDestroyerCount++;

		if (CurrentDestroyerCount >= DestroyerToSpawn)
		{
			LevelEnd();
		}
	}

}
#include "Framework/World.h"
#include "Levels/DestroyerLevel.h"
#include "Enemy/Destroyer.h"
#include "Framework/MathUtility.h"

namespace we
{
    DestroyerLevel::DestroyerLevel(World* World)
        : Level{ World }
        , MinInterval{ 4.0f }
        , MaxInterval{ 7.5f }
        , SpawnLocationL{ World->GetWindowSize().x * (1.f / 5.f), -40.f }
        , SpawnLocationR{ World->GetWindowSize().x * (4.f / 5.f), -40.f }
        , DestroyerToSpawn{ 7 }
        , CurrentDestroyerCount{ 0 }
    {
    }

    void DestroyerLevel::BeginLevel()
    {
        SpawnDestroyer();
    }

    void DestroyerLevel::EndLevel()
    {
        TimerManager::Get().ClearTimer(SpawnTimerHandle);
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

        sf::Vector2u WindowSize = GetWorld()->GetWindowSize();

        float RandomX = we::RandomRange(50.f, static_cast<float>(WindowSize.x) - 50.f);

        float RandomY = we::RandomRange(-100.f, -50.f);

        DestroyerSpawned->SetActorLocation({ RandomX, RandomY });

        DestroyerSpawned->GetHealthComponent().SetInitialHealth(300.f, 300.f);

        CurrentDestroyerCount++;

        if (CurrentDestroyerCount >= DestroyerToSpawn)
        {
            LevelEnd();
            return;
        }

        TimerManager::Get().ClearTimer(SpawnTimerHandle);
        float NextInterval = we::RandomRange(MinInterval, MaxInterval);
        SpawnTimerHandle = TimerManager::Get().SetTimer(GetObject(), &DestroyerLevel::SpawnDestroyer, NextInterval, true);
    }
}
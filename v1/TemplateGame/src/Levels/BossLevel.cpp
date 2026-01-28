#include "Levels/BossLevel.h"
#include "Enemy/Boss.h"
#include "Framework/World.h"
#include "GameFramework/PlayerManager.h"

namespace we
{
    BossLevel::BossLevel(World* World)
        : Level{ World }
        , EntranceTimer{ 0.f }
    {
    }

    void BossLevel::BeginLevel()
    {
        weak<Boss> NewBoss = GetWorld()->SpawnActor<Boss>();
        CurrentBoss = NewBoss;
        PlayerManager::Get().SetBoss(NewBoss);
        NewBoss.lock()->SetActorLocation({ 300.f, -300.f });
        NewBoss.lock()->OnActorDestroyed.Bind(GetObject(), &BossLevel::BossDefeated);
    }

    void BossLevel::TickLevel(float DeltaTime)
    {
        if (!CurrentBoss.expired())
        {
            EntranceSequence(DeltaTime);
        }
    }

    void BossLevel::EntranceSequence(float DeltaTime)
    {
        auto Boss = CurrentBoss.lock();
        if (!Boss) return;

        sf::Vector2f Loc = Boss->GetActorLocation();
        EntranceTimer += DeltaTime;

        if (Loc.y < 140.f)
        {
            float EntranceSpeed = 300.f;
            Boss->SetActorLocation({ Loc.x, Loc.y + (EntranceSpeed * DeltaTime) });
        }
    }

    void BossLevel::BossDefeated(Actor* BossActor)
    {
        PlayerManager::Get().SetBoss(weak<Boss>{});
        LevelEnd();
    }
}
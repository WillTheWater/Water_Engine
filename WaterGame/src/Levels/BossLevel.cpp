#include "Levels/BossLevel.h"
#include "Enemy/Boss.h"
#include "Framework/World.h"

namespace we
{
	BossLevel::BossLevel(World* World)
		: Level{World}
	{
	}

	void BossLevel::BeginLevel()
	{
		weak<Boss> NewBoss = GetWorld()->SpawnActor<Boss>();
		NewBoss.lock()->SetActorLocation({ 300.f, 140.f });
		NewBoss.lock()->OnActorDestroyed.Bind(GetObject(), &BossLevel::BossDefeated);
	}

	void BossLevel::BossDefeated(Actor* BossActor)
	{
		LevelEnd();
	}
}
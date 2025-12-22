#include "Enemy/EnemySpaceship.h"
#include "Framework/EActorTypes.h"
#include "Framework/MathUtility.h"
#include "GameFramework/PlayerManager.h"

namespace we
{
	EnemySpaceship::EnemySpaceship(World* OwningWorld, const string& TexturePath, float CollisionDamage, const List<PickupFactoryFunction> Pickups)
		: Spaceship{OwningWorld, TexturePath}
		, CollisionDamage{CollisionDamage}
		, Pickups{Pickups}
	{
		SetActorID(EActorID::Enemy);
	}

	void EnemySpaceship::Tick(float DeltTime)
	{
		Spaceship::Tick(DeltTime);
		if (IsActorOutOfBounds(GetSpriteBounds().size.x * 2.f))
		{
			Destroy();
		}
	}

	void EnemySpaceship::SpawnDrops()
	{
		if (Pickups.size() == 0) { return; }
		int pick = (int)RandomRange(0, Pickups.size());
		if (pick >= 0 && pick < Pickups.size())
		{
			weak<Pickup> NewPickup = Pickups[pick](GetWorld());
			auto Test = NewPickup.lock();
			if (Test)
			{
				Test->SetActorLocation(GetActorLocation());
			}
		}
		Player* Player = PlayerManager::Get().GetPlayer();
		if (Player)
		{
			Player->AddScore(5);
		}
	}

	void EnemySpaceship::OnActorBeginOverlap(Actor* OtherActor)
	{
		Spaceship::OnActorBeginOverlap(OtherActor);
		if (IsHostile(OtherActor))
		{
			OtherActor->Damage(CollisionDamage);
		}
	}
}

#include "Enemy/EnemySpaceship.h"
#include "Framework/EActorTypes.h"

namespace we
{
	EnemySpaceship::EnemySpaceship(World* OwningWorld, const string& TexturePath, float CollisionDamage)
		: Spaceship{OwningWorld, TexturePath}
		, CollisionDamage{CollisionDamage}
	{
		SetActorID(EActorID::Enemy);
	}

	void EnemySpaceship::Tick(float DeltTime)
	{
		Spaceship::Tick(DeltTime);
		if (IsOutOfBounds(GetSpriteBounds().size.x * 2.f))
		{
			Destroy();
		}
	}

	void EnemySpaceship::OnActorBeginOverlap(Actor* OtherActor)
	{
		Spaceship::OnActorBeginOverlap(OtherActor);
		LOG("Overlaped")
		if (IsHostile(OtherActor))
		{
			OtherActor->Damage(CollisionDamage);
			LOG("Hit")
		}
	}
}

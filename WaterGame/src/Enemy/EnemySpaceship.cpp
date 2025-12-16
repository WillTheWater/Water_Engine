#include "Enemy/EnemySpaceship.h"

namespace we
{
	EnemySpaceship::EnemySpaceship(World* OwningWorld, const string& TexturePath, float CollisionDamage)
		: Spaceship{OwningWorld, TexturePath}
		, CollisionDamage{CollisionDamage}
	{
	
	}

	void EnemySpaceship::Tick(float DeltTime)
	{
		Spaceship::Tick(DeltTime);
		if (IsOutOfBounds(GetSpriteBounds().size.x))
		{
			Destroy();
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

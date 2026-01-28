#include "VFX/Explosion.h"
#include "Framework/MathUtility.h"
#include "Framework/World.h"
#include "VFX/Partical.h"

namespace we
{
	Explosion::Explosion(int ParticalAmount, float MinLife, float MaxLife, float MinSize, float MaxSize, float MinSpeed, float MaxSpeed, const sf::Color& ParticalColor, const List<string>& ParticalImagePaths)
		: ParticalCount{ParticalAmount}
		, LifetimeMin{MinLife}
		, LifetimeMax{MaxLife}
		, SizeMin{MinSize}
		, SizeMax{MaxSize}
		, SpeedMin{MinSpeed}
		, SpeedMax{MaxSpeed}
		, ParticalColor{ ParticalColor }
		, ParticalImagePaths{ ParticalImagePaths }
	{
	}

	void Explosion::SpawnExplosion(World* World, const sf::Vector2f& SpawnLocation)
	{
		if (!World) { return; }
		for (int i = 0; i < ParticalCount; i++)
		{
			string ParticalPath = ParticalImagePaths[(int)RandomRange(0, ParticalImagePaths.size())];
			weak<Partical> NewPartical = World->SpawnActor<Partical>(ParticalPath);
			auto ParticleShared = NewPartical.lock();
			if (!ParticleShared)
			{
				continue;
			}
			ParticleShared->RandomLifetime(LifetimeMin, LifetimeMax);
			ParticleShared->SetActorLocation(SpawnLocation);
			ParticleShared->RandomSize(SizeMin, SizeMax);
			ParticleShared->RandomVelocity(SpeedMin, SpeedMax);
			ParticleShared->GetSprite().setColor(sf::Color{ (uint8_t)RandomRange(0,255), (uint8_t)RandomRange(0,255), (uint8_t)RandomRange(0,255), (uint8_t)255 });
		}
	}
}
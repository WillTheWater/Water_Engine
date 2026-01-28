#include "VFX/Partical.h"
#include "Framework/MathUtility.h"

namespace we
{
	Partical::Partical(World* OwningWorld, const string& TexturePath)
		: Actor{OwningWorld, TexturePath}
		, Velocity{}
		, Lifetime{}
		, Timer{}
	{
	}

	void Partical::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);

		Move(DeltaTime);
		Fade(DeltaTime);
		HandleLifetime();
	}

	void Partical::HandleLifetime()
	{
		if (Timer.getElapsedTime().asSeconds() >= Lifetime)
		{
			Destroy();
		}
	}

	void Partical::RandomVelocity(float Min, float Max)
	{
		Velocity = RandomUnitVector() * RandomRange(Min, Max);
		SetActorRotation(VectorToRotation(Velocity));
	}

	void Partical::RandomSize(float Min, float Max)
	{
		float RandomScale = RandomRange(Min, Max);
		GetSprite().setScale({ RandomScale, RandomScale });
	}

	void Partical::RandomLifetime(float Min, float Max)
	{
		Lifetime = RandomRange(Min, Max);
	}

	void Partical::Move(float DeltaTime)
	{
		AddActorLocationOffset(Velocity * DeltaTime);
	}

	void Partical::Fade(float DeltaTime)
	{
		float ElapsedTime = Timer.getElapsedTime().asSeconds()/Lifetime;
		GetSprite().setColor(LerpColor(GetSprite().getColor(), sf::Color(0,0,0,0), ElapsedTime));
		GetSprite().setScale(LerpVector(GetSpriteScale(), sf::Vector2f{2.5f, 2.5f}, ElapsedTime));
	}
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Actor.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	Actor::Actor(World* OwningWorld, const string& TexturePath)
		: OwnerWorld{OwningWorld}
		, Texture{ Asset().LoadTexture(TexturePath) }
		, Sprite{*Texture}
		, bHasBegunPlay{false}
		, Position{}
	{
		SetTexture();
	}

	void Actor::BeginPlay()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
		}
	}

	void Actor::Tick(float DeltaTime)
	{
	}

	void Actor::Destroy()
	{
		OnActorDestroyed.Broadcast(this);
	}

	void Actor::SetTexture()
	{
		if (!Texture) { return; }
		Sprite.setTexture(*Texture);

		int textureWidth = Texture->getSize().x;
		int textureHeight = Texture->getSize().y;
		Sprite.setTextureRect(sf::IntRect{ sf::Vector2i{}, sf::Vector2i{textureWidth, textureHeight} });
	}

	void Actor::SetPosition(vec2f Pos)
	{
		Position = Pos;
		Sprite.setPosition(Position);
	}
}
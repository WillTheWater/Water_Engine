// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Actor.h"
#include "Framework/World/World.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	Actor::Actor(World* InWorld, const string& TexturePath)
		: OwningWorld{ InWorld }
		, ActorTexture{ TexturePath.empty() ? nullptr : LoadAsset().LoadTextureSync(TexturePath) }
		, ActorSprite{ ActorTexture ? *ActorTexture : *LoadAsset().GetPlaceholderTexture() }
		, ActorPosition{}
		, ActorRotation{}
		, ActorScale{ 1.0f, 1.0f }
		, bIsVisible{ true }
	{
		Initialize();
	}

	void Actor::BeginPlay()
	{
		// Prevent double BeginPlay
		if (bHasBegunPlay) return;
		bHasBegunPlay = true;
	}

	void Actor::Tick(float DeltaTime)
	{
		// Don't tick until BeginPlay has been called
		if (!bHasBegunPlay) return;
	}

	const sf::Drawable* Actor::GetDrawable() const
	{
		return bIsVisible ? &ActorSprite : nullptr;
	}

	void Actor::SetPosition(const vec2f& NewPosition)
	{
		ActorPosition = NewPosition;
		UpdateSpriteTransform();
	}

	void Actor::SetRotation(angle NewRotation)
	{
		ActorRotation = NewRotation;
		UpdateSpriteTransform();
	}

	void Actor::SetScale(const vec2f& NewScale)
	{
		ActorScale = NewScale;
		UpdateSpriteTransform();
	}

	void Actor::Initialize()
	{
		auto bounds = ActorSprite.getLocalBounds();
		ActorSprite.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
	}

	void Actor::SetSpriteTexture(shared<texture> NewTexture)
	{
		if (NewTexture)
		{
			ActorTexture = NewTexture;
			ActorSprite.setTexture(*ActorTexture);
		}
	}

	void Actor::SetSpriteTextureRect(const recti& TexRect)
	{
		ActorSprite.setTextureRect(TexRect);
	}

	void Actor::SetOrigin(const vec2f& Origin)
	{
		ActorSprite.setOrigin(Origin);
	}

	void Actor::UpdateSpriteTransform()
	{
		ActorSprite.setPosition(ActorPosition);
		ActorSprite.setRotation(ActorRotation);
		ActorSprite.setScale(ActorScale);
	}
}
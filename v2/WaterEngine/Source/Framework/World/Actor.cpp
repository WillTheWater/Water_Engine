// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Math.h"

namespace we
{
	Actor::Actor(World* InWorld, const string& TexturePath)
		: Object()
		, OwningWorld{ InWorld }
		, ActorPosition{}
		, ActorRotation{}
		, ActorScale{ 1.0f, 1.0f }
		, bIsVisible{ true }
	{
		// Initialize sprite with placeholder or provided texture
		if (!TexturePath.empty() && OwningWorld)
		{
			// TODO: Load texture via ResourceSubsystem
			// For now, create empty sprite (will be set when texture loads)
		}
		
		Initialize();
	}

	void Actor::BeginPlay()
	{
		if (bHasBegunPlay) return;
		bHasBegunPlay = true;
	}

	void Actor::Tick(float DeltaTime)
	{
		if (!bHasBegunPlay) return;
	}

	const drawable* Actor::GetDrawable() const
	{
		return (bIsVisible && ActorSprite.has_value()) ? &ActorSprite.value() : nullptr;
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
		// Sprite will be initialized when texture is set
	}

	void Actor::SetTexture(shared<texture> NewTexture)
	{
		if (NewTexture)
		{
			ActorTexture = NewTexture;
			
			// Create or recreate sprite with new texture
			ActorSprite.emplace(*ActorTexture);
			
			// Reset origin to center of new texture
			auto bounds = ActorSprite->getLocalBounds();
			ActorSprite->setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
			
			// Apply current transform
			UpdateSpriteTransform();
		}
	}

	void Actor::SetTextureRect(const recti& TexRect)
	{
		if (ActorSprite) ActorSprite->setTextureRect(TexRect);
	}

	void Actor::SetOrigin(const vec2f& Origin)
	{
		if (ActorSprite) ActorSprite->setOrigin(Origin);
	}

	void Actor::UpdateSpriteTransform()
	{
		if (ActorSprite)
		{
			ActorSprite->setPosition(ActorPosition);
			ActorSprite->setRotation(ActorRotation);
			ActorSprite->setScale(ActorScale);
		}
	}
}

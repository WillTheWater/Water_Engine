// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor.h"
#include "Framework/World/World.h"
#include "Utility/Math.h"

namespace we
{
	Actor::Actor(World* InWorld)
		: Object()
		, OwningWorld{ InWorld }
		, ActorPosition{}
		, ActorRotation{}
		, ActorScale{ 1.0f, 1.0f }
		, bIsVisible{ true }
	{
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
		if (!bIsVisible) return nullptr;
		
		// Prefer shape for grey boxing, fallback to sprite
		if (ActorShape) return ActorShape.get();
		if (ActorSprite.has_value()) return &ActorSprite.value();
		
		return nullptr;
	}

	void Actor::SetPosition(const vec2f& NewPosition)
	{
		ActorPosition = NewPosition;
		UpdateTransform();
	}

	void Actor::SetRotation(angle NewRotation)
	{
		ActorRotation = NewRotation;
		UpdateTransform();
	}

	void Actor::SetScale(const vec2f& NewScale)
	{
		ActorScale = NewScale;
		UpdateTransform();
	}

	// =========================================================================
	// Sprite
	// =========================================================================

	void Actor::SetTexture(shared<texture> NewTexture)
	{
		if (NewTexture)
		{
			ActorTexture = NewTexture;
			
			// Create sprite with new texture
			ActorSprite.emplace(*ActorTexture);
			
			// Center origin
			auto bounds = ActorSprite->getLocalBounds();
			ActorSprite->setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
			
			UpdateTransform();
		}
	}

	void Actor::SetTextureRect(const recti& TexRect)
	{
		if (ActorSprite) ActorSprite->setTextureRect(TexRect);
	}

	void Actor::SetSpriteOrigin(const vec2f& Origin)
	{
		if (ActorSprite) ActorSprite->setOrigin(Origin);
	}

	// =========================================================================
	// Shape (Grey Boxing)
	// =========================================================================

	void Actor::SetShape(unique<shape> NewShape)
	{
		ActorShape = std::move(NewShape);
		UpdateTransform();
	}

	void Actor::SetShapeColor(const color& Color)
	{
		if (ActorShape)
		{
			ActorShape->setFillColor(Color);
		}
	}

	void Actor::SetAsRectangle(vec2f Size, color FillColor)
	{
		auto rect = make_unique<rectangle>(Size);
		rect->setFillColor(FillColor);
		rect->setOrigin({ Size.x / 2.0f, Size.y / 2.0f });
		
		ActorShape = std::move(rect);
		UpdateTransform();
	}

	void Actor::SetAsCircle(float Radius, color FillColor)
	{
		auto circ = make_unique<circle>(Radius);
		circ->setFillColor(FillColor);
		circ->setOrigin({ Radius, Radius });
		
		ActorShape = std::move(circ);
		UpdateTransform();
	}

	// =========================================================================
	// Transform Update
	// =========================================================================

	void Actor::UpdateTransform()
	{
		// Update shape
		if (ActorShape)
		{
			ActorShape->setPosition(ActorPosition);
			ActorShape->setRotation(ActorRotation);
			ActorShape->setScale(ActorScale);
		}
		
		// Update sprite
		if (ActorSprite)
		{
			ActorSprite->setPosition(ActorPosition);
			ActorSprite->setRotation(ActorRotation);
			ActorSprite->setScale(ActorScale);
		}
	}
}

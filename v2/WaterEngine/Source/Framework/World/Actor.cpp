// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor.h"
#include "Utility/Log.h"

namespace we
{
	Actor::Actor(World& OwningWorld)
		: OwningWorld{OwningWorld}
		, Position{}
		, Rotation{}
		, Scale{1.f, 1.f}
		, ActorSprite{}
		, CustomDepth{}
		, bIsVisible{true}
		, bHasBegunPlay{false}
	{
	}

	Actor::~Actor()
	{
		LOG("Actor Destroyed")
	}

	void Actor::StartPlay()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
			BeginPlay();
		}
	}

	void Actor::StartTick(float DeltaTime)
	{
		if (!IsPendingDestroy())
		{
			Tick(DeltaTime);
		}
	}

	void Actor::BeginPlay()
	{
		LOG("Actor BeginPlay")
	}

	void Actor::Tick(float DeltaTime)
	{
		LOG("Actor Tick")
	}

	void Actor::UpdateTransform()
	{
		if (HasSprite())
		{
			ActorSprite->setPosition(Position);
			ActorSprite->setRotation(Rotation);
			ActorSprite->setScale(Scale);
		}
	}

	void Actor::SetSprite(shared<texture> Texture)
	{
		ActorSprite->setTexture(*Texture);
	}

	void Actor::SetSpriteOrigin(const vec2f& Origin)
	{
		if (HasSprite())
		{
			ActorSprite->setOrigin(Origin);
		}
	}

	const drawable* Actor::GetDrawable() const
	{
		if (!bIsVisible) return nullptr;

		if (ActorSprite.has_value()) return &ActorSprite.value();

		return nullptr;
	}
}
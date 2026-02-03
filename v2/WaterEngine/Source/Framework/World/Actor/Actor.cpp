// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Actor.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	Actor::Actor(World* OwningWorld, const string& TexturePath)
		: OwnerWorld{OwningWorld}
		, Texture{ Asset().LoadTexture(TexturePath) }
		, Sprite{*Texture}
		, bHasBegunPlay{false}
	{
	}

	Actor::~Actor()
	{
		LOG("Actor Destroyed")
	}

	void Actor::BeginPlayGlobal()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
			BeginPlay();
		}
	}

	void Actor::Destroy()
	{
		OnActorDestroyed.Broadcast(this);
		Object::Destroy();
	}

	void Actor::SetPosition(const vec2f& Position)
	{
		Sprite.setPosition(Position);
	}

	vec2f Actor::GetPosition() const
	{
		return Sprite.getPosition();
	}

	void Actor::SetRotation(angle Angle)
	{
		Sprite.setRotation(Angle);
	}

	angle Actor::GetRotation() const
	{
		return Sprite.getRotation();
	}

	void Actor::SetScale(const vec2f& Scale)
	{
		Sprite.setScale(Scale);
	}

	vec2f Actor::GetScale() const
	{
		return Sprite.getScale();
	}

	rectf Actor::GetBounds() const
	{
		return Sprite.getGlobalBounds();
	}

	void Actor::SetOrigin(const vec2f& Origin)
	{
		Sprite.setOrigin(Origin);
	}

	void Actor::CenterOrigin()
	{
		SetOrigin({ Sprite.getGlobalBounds().size.x / 2, Sprite.getGlobalBounds().size.y / 2 });
	}

	void Actor::SetColor(const color& Color)
	{
		Sprite.setColor(Color);
	}
}
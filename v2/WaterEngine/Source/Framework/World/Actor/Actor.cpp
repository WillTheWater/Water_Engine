// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Actor.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Interface/Component/IActorComponent.h"
#include "Utility/Log.h"

namespace we
{
	Actor::Actor(World* OwningWorld)
		: OwnerWorld{ OwningWorld }
		, bHasBegunPlay{ false }
	{
	}

	Actor::Actor(World* OwningWorld, const string& TexturePath)
		: OwnerWorld{OwningWorld}
		, Texture{ Asset().LoadTexture(TexturePath) }
		, Sprite{*Texture}
		, bHasBegunPlay{false}
	{
		if (Texture)
		{
			Sprite.emplace(*Texture);
		}
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

	void Actor::BeginPlay()
	{
		for (auto& Comp : Components)
		{
			Comp->BeginPlay();
		}
	}

	void Actor::Tick(float DeltaTime)
	{
		for (auto& Comp : Components)
		{
			Comp->Tick(DeltaTime);
		}
	}

	void Actor::Destroy()
	{
		for (auto& Comp : Components)
		{
			Comp->EndPlay();
		}
		Components.clear();
		OnActorDestroyed.Broadcast(this);
		Object::Destroy();
	}

	void Actor::SetPosition(const vec2f& Position)
	{
		if (Sprite) Sprite->setPosition(Position);
	}

	vec2f Actor::GetPosition() const
	{
		return Sprite ? Sprite->getPosition() : vec2f{};
	}

	void Actor::SetRotation(angle Angle)
	{
		if (Sprite) Sprite->setRotation(Angle);
	}

	angle Actor::GetRotation() const
	{
		return Sprite ? Sprite->getRotation() : angle{};
	}

	void Actor::SetScale(const vec2f& Scale)
	{
		if (Sprite) Sprite->setScale(Scale);
	}

	vec2f Actor::GetScale() const
	{
		return Sprite ? Sprite->getScale() : vec2f{ 1, 1 };
	}

	rectf Actor::GetBounds() const
	{
		return Sprite ? Sprite->getGlobalBounds() : rectf{};
	}

	void Actor::SetOrigin(const vec2f& Origin)
	{
		if (Sprite) Sprite->setOrigin(Origin);
	}

	void Actor::CenterOrigin()
	{
		if (!Sprite) return;
		auto LocalBounds = Sprite->getLocalBounds();
		Sprite->setOrigin({ LocalBounds.size.x / 2, LocalBounds.size.y / 2 });
	}

	void Actor::SetColor(const color& Color)
	{
		if (Sprite) Sprite->setColor(Color);
	}

	void Actor::AddComponent(shared<IActorComponent> Component)
	{
		Components.push_back(std::move(Component));
	}
}
//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#include "Framework/World/Actor/Actor.h"
//#include "Subsystem/ResourceSubsystem.h"
//#include "Interface/Component/IActorComponent.h"
//#include "Framework/World/World.h"
//#include "Utility/Log.h"
//
//namespace we
//{
//	Actor::Actor(World* OwningWorld)
//		: OwnerWorld{ OwningWorld }
//		, Position{}
//		, Rotation{}
//		, Scale{ 1.f, 1.f }
//		, Velocity{ 0.f, 0.f }
//		, bHasBegunPlay{ false }
//	{
//	}
//
//	Actor::Actor(World* OwningWorld, const string& TexturePath)
//		: OwnerWorld{OwningWorld}
//		, Texture{ Asset().LoadTexture(TexturePath) }
//		, Sprite{*Texture}
//		, Position{}
//		, Rotation{}
//		, Scale{ 1.f, 1.f }
//		, bHasBegunPlay{false}
//	{
//		if (Texture)
//		{
//			Sprite.emplace(*Texture);
//		}
//	}
//
//	Actor::~Actor()
//	{
//		LOG("Actor Destroyed")
//	}
//
//	void Actor::BeginPlayGlobal()
//	{
//		if (!bHasBegunPlay)
//		{
//			bHasBegunPlay = true;
//			BeginPlay();
//		}
//	}
//
//	void Actor::BeginPlay()
//	{
//		for (auto& Comp : Components)
//		{
//			Comp->BeginPlay();
//		}
//	}
//
//	void Actor::Tick(float DeltaTime)
//	{
//		for (auto& Comp : Components)
//		{
//			Comp->Tick(DeltaTime);
//		}
//	}
//
//	void Actor::Destroy()
//	{
//		for (auto& Comp : Components)
//		{
//			Comp->EndPlay();
//		}
//		Components.clear();
//		OnActorDestroyed.Broadcast(this);
//		Object::Destroy();
//	}
//
//	void Actor::SetPosition(const vec2f& NewPosition)
//	{
//		Position = NewPosition;
//		if (Sprite) Sprite->setPosition(Position);
//	}
//
//	vec2f Actor::GetPosition() const
//	{
//		return Position;
//	}
//
//	void Actor::SetRotation(angle NewAngle)
//	{
//		Rotation = NewAngle;
//		if (Sprite) Sprite->setRotation(Rotation);
//	}
//
//	angle Actor::GetRotation() const
//	{
//		return Rotation;
//	}
//
//	void Actor::SetScale(const vec2f& NewScale)
//	{
//		Scale = NewScale;
//		if (Sprite) Sprite->setScale(Scale);
//	}
//
//	vec2f Actor::GetScale() const
//	{
//		return Scale;
//	}
//
//	rectf Actor::GetBounds() const
//	{
//		return Sprite ? Sprite->getGlobalBounds() : rectf{};
//	}
//
//	void Actor::SetOrigin(const vec2f& Origin)
//	{
//		if (Sprite) Sprite->setOrigin(Origin);
//	}
//
//	void Actor::CenterOrigin()
//	{
//		if (!Sprite) return;
//		auto LocalBounds = Sprite->getLocalBounds();
//		Sprite->setOrigin({ LocalBounds.size.x / 2, LocalBounds.size.y / 2 });
//	}
//
//	void Actor::SetColor(const color& Color)
//	{
//		if (Sprite) Sprite->setColor(Color);
//	}
//
//	void Actor::SetSpriteTexture(shared<texture> NewTexture)
//	{
//		Texture = NewTexture;
//		if (Texture && Sprite)
//		{
//			Sprite->setTexture(*Texture);
//		}
//	}
//
//	void Actor::AddComponent(shared<IActorComponent> Component)
//	{
//		Components.push_back(std::move(Component));
//	}
//}
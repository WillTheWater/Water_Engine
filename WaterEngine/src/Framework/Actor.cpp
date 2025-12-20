#include "Framework/Actor.h"
#include "Framework/Core.h"
#include "Framework/AssetManager.h"
#include "Framework/MathUtility.h"
#include "Framework/World.h"
#include "Framework/PhysicsSystem.h"
#include "Framework/Renderer.h"
#include "box2d/b2_body.h"
#include "Framework/EActorTypes.h"

namespace we
{
	Actor::Actor(World* OwningWorld, const string& TexturePath)
		: OwningWorld{ OwningWorld }
		, bHasBegunPlay{ false }
		, Texture{}
		, Sprite{ nullptr }
		, ActorLocation{}
		, ActorRotation{}
		, ActorScale{ 1.f, 1.f }
		, SpriteLocationOffset{ 0.f, 0.f }
		, SpriteRotationOffset{ sf::degrees(0.f) }
		, SpriteScale{ 1.f, 1.f }
		, PhysicsBody{nullptr}
		, bPhysicsEnabled{ false }
		, ActorID{GetNeutralActorID()}
		, bRenderDebugShapes{false}
	{
		SetTexture(TexturePath);
	}

	Actor::~Actor()
	{
	}

	void Actor::BeginPlayGlobal()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
			BeginPlay();
		}
	}

	void Actor::TickGlobal(float DeltaTime)
	{
		if (!IsPendingDestroy())
		{
			Tick(DeltaTime);
		}
	}

	void Actor::BeginPlay()
	{
	}

	void Actor::Tick(float DeltaTime)
	{
	}

	void Actor::Destroy()
	{
		UninitializePhysics();
		OnActorDestroyed.Broadcast(this);
		Object::Destroy();
	}

	void Actor::Render(Renderer& GameRenderer)
	{
		if (IsPendingDestroy() || !Sprite) { return; }

		UpdateSpriteTransform();
		GameRenderer.DrawSprite(*Sprite);

		if (bRenderDebugShapes)
		{
			GameRenderer.DrawDebugShape(ForwardVectorDebugShape());
			GameRenderer.DrawDebugShape(CollisionShapeDebug());
		}
	}

	sf::Vector2u Actor::GetWindowSize() const
	{
		return OwningWorld->GetWindowSize();
	}

	void Actor::SetTexture(const string& TexturePath)
	{
		Texture = AssetManager::Get().LoadTexture(TexturePath);
		if (!Texture) { return; }

		Sprite = std::make_shared<sf::Sprite>(*Texture);

		CenterPivot();
	}

	void Actor::SetSpriteLocationOffset(const sf::Vector2f& Position)
	{
		SpriteLocationOffset = Position;
	}

	void Actor::SetSpriteRotationOffset(const sf::Angle& Rotation)
	{
		SpriteRotationOffset = Rotation;
	}

	void Actor::SetSpriteScale(const sf::Vector2f& Scale)
	{
		SpriteScale = Scale;
	}

	void Actor::SetSpriteFrame(int FrameWidth, int FrameHeight)
	{
		if (!Sprite || !Texture) { return; }

		if (FrameWidth > 0 && FrameHeight > 0)
		{
			Sprite->setTextureRect(sf::IntRect({ 0, 0 }, { FrameWidth, FrameHeight }));
		}
		else
		{
			int TextureWidth = static_cast<int>(Texture->getSize().x);
			int TextureHeight = static_cast<int>(Texture->getSize().y);
			Sprite->setTextureRect(sf::IntRect({ 0, 0 }, { TextureWidth, TextureHeight }));
		}

		CenterPivot();
	}

	bool Actor::IsActorOutOfBounds(float Allowance) const
	{
		const sf::Vector2u window = GetWindowSize();
		const sf::Vector2f pos = GetActorLocation();
		const sf::Vector2f ext = GetActorExtents();

		return
			pos.x < -ext.x - Allowance ||
			pos.x > window.x + ext.x + Allowance ||
			pos.y < -ext.y - Allowance ||
			pos.y > window.y + ext.y + Allowance;
	}

	void Actor::SetPhysicsEnabled(bool Enabled)
	{
		bPhysicsEnabled = Enabled;
		if (bPhysicsEnabled)
		{
			InitializePhysics();
		}
		else
		{
			UninitializePhysics();
		}
	}

	void Actor::InitializePhysics()
	{
		if (!PhysicsBody)
		{
			PhysicsBody = PhysicsSystem::Get().AddListener(this);
		}
	}

	void Actor::UninitializePhysics()
	{
		if (PhysicsBody)
		{
			PhysicsSystem::Get().RemoveListener(PhysicsBody);
			PhysicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(nullptr);
			PhysicsBody = nullptr;
		}
	}

	void Actor::UpdatePhysicsBodyTransform()
	{
		if (PhysicsBody)
		{
			float PhysicsScale = PhysicsSystem::Get().GetPhysicsScale();
			b2Vec2 Pos{ GetActorLocation().x * PhysicsScale, GetActorLocation().y * PhysicsScale };
			float Rot = GetActorRotation().asRadians();
			PhysicsBody->SetTransform(Pos, Rot);
		}
	}
	
	void Actor::OnActorBeginOverlap(Actor* OtherActor)
	{
	}

	void Actor::OnActorEndOverlap(Actor* OtherActor)
	{
	}

	bool Actor::IsHostile(Actor* OtherActor)
	{
		if (!OtherActor) { return false; }

		const EActorID MyID = GetActorID();
		const EActorID OtherID = OtherActor->GetActorID();

		if (MyID == EActorID::Neutral || OtherID == EActorID::Neutral || MyID == OtherID) { return false; }

		return true;
	}

	void Actor::Damage(float Amount)
	{
	}

	void Actor::SetActorLocation(const sf::Vector2f& NewLocation)
	{
		ActorLocation = NewLocation;
		UpdatePhysicsBodyTransform();
	}

	void Actor::SetActorRotation(const sf::Angle& NewRotation)
	{
		ActorRotation = NewRotation;
		UpdatePhysicsBodyTransform();
	}

	void Actor::SetActorScale(const sf::Vector2f NewScale)
	{
		ActorScale = NewScale;
	}

	void Actor::SetActorExtents(const sf::Vector2f& HalfSize)
	{
		ActorExtents = HalfSize;
		SizeSource = EActorSize::Manual;
	}

	void Actor::SetActorSizeSource(EActorSize Source)
	{
		SizeSource = Source;
	}

	sf::Vector2f Actor::GetActorExtents() const
	{
		switch (SizeSource)
		{
		case EActorSize::SpriteBounds:
		{
			if (!Sprite) { return ActorExtents; }
			const auto bounds = GetSprite().getGlobalBounds();
			return { bounds.size.x * 0.5f, bounds.size.y * 0.5f };
		}

		case EActorSize::Manual:
		default:
			return ActorExtents;
		}
	}

	void Actor::AddActorLocationOffset(const sf::Vector2f& Offset)
	{
		SetActorLocation(GetActorLocation() + Offset);
	}

	void Actor::AddActorRotationOffset(const sf::Angle& RotOffset)
	{
		SetActorRotation(GetActorRotation() + RotOffset);
	}

	sf::Vector2f Actor::GetActorForwardVector() const
	{
		return RotationToVector(ActorRotation);
	}

	sf::Vector2f Actor::GetActorRightVector() const
	{
		sf::Angle rightRot = ActorRotation + sf::degrees(90.f);
		return RotationToVector(rightRot);
	}

	sf::FloatRect Actor::GetSpriteBounds() const
	{
		return Sprite->getGlobalBounds();
	}

	void Actor::UpdateSpriteTransform()
	{
		if (!Sprite) return;

		Sprite->setPosition(ActorLocation + SpriteLocationOffset);
		Sprite->setRotation(ActorRotation + SpriteRotationOffset);
		Sprite->setScale({ ActorScale.x * SpriteScale.x, ActorScale.y * SpriteScale.y });
	}

	void Actor::CenterPivot()
	{
		if (!Sprite) { return; }
		sf::FloatRect localBounds = Sprite->getLocalBounds();
		Sprite->setOrigin({ localBounds.size.x / 2.f, localBounds.size.y / 2.f });
	}

	sf::VertexArray Actor::ForwardVectorDebugShape(float Length, sf::Color Color) const
	{
		const sf::Vector2f start = GetActorLocation();
		const sf::Vector2f forward = GetActorForwardVector();
		const sf::Vector2f end = start + forward * Length;

		sf::VertexArray arrow(sf::PrimitiveType::Lines);

		arrow.append({ start, Color });
		arrow.append({ end,   Color });

		constexpr float HeadSize = 10.f;
		constexpr float HeadAngleDeg = 25.f;
		const sf::Angle forwardRot = VectorToRotation(forward);

		const sf::Vector2f left = end - RotationToVector(forwardRot - sf::degrees(HeadAngleDeg)) * HeadSize;
		const sf::Vector2f right = end - RotationToVector(forwardRot + sf::degrees(HeadAngleDeg)) * HeadSize;

		arrow.append({ end, Color });
		arrow.append({ left, Color });

		arrow.append({ end, Color });
		arrow.append({ right, Color });

		return arrow;
	}

	sf::VertexArray Actor::CollisionShapeDebug(sf::Color Color) const
	{
		sf::Vector2f ext = GetActorExtents();
		sf::Vector2f center = GetActorLocation();

		sf::VertexArray rect(sf::PrimitiveType::LineStrip, 5);
		rect[0].position = { center.x - ext.x, center.y - ext.y };
		rect[1].position = { center.x + ext.x, center.y - ext.y };
		rect[2].position = { center.x + ext.x, center.y + ext.y };
		rect[3].position = { center.x - ext.x, center.y + ext.y };
		rect[4].position = rect[0].position;

		for (int i = 0; i < 5; ++i)
			rect[i].color = Color;

		return rect;
	}
}
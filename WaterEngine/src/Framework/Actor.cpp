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
		, ATexture{}
		, ASprite{ nullptr }
		, PhysicsBody{nullptr}
		, bPhysicsEnabled{ false }
		, ActorID{GetNeutralActorID()}
	{
		SetTexture(TexturePath);
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
		UninitialziePhysics();
		Object::Destroy();
	}

	sf::Vector2u Actor::GetWindowSize() const
	{
		return OwningWorld->GetWindowSize();
	}

	void Actor::SetTexture(const string& TexturePath, float SpriteScale)
	{
		ATexture = AssetManager::Get().LoadTexture(TexturePath);
		if (!ATexture) { return; }

		ASprite = std::make_shared<sf::Sprite>(*ATexture);

		ASprite->setScale({ SpriteScale, SpriteScale });

		CenterPivot();
	}

	void Actor::SetSpriteFrame(int FrameWidth, int FrameHeight)
	{
		if (!ASprite || !ATexture) { return; }

		FrameSize = { FrameWidth, FrameHeight };

		if (FrameWidth > 0 && FrameHeight > 0)
		{
			ASprite->setTextureRect(sf::IntRect({ 0, 0 }, { FrameWidth, FrameHeight }));
		}
		else
		{
			int TextureWidth = static_cast<int>(ATexture->getSize().x);
			int TextureHeight = static_cast<int>(ATexture->getSize().y);
			ASprite->setTextureRect(sf::IntRect({ 0, 0 }, { TextureWidth, TextureHeight }));
		}

		CenterPivot();
	}

	void Actor::SetActorScale(float NewScale)
	{
		if (!ASprite) { return; }

		ASprite->setScale({ NewScale, NewScale });
	}

	void Actor::Render(Renderer& GameRenderer)
	{
		if (IsPendingDestroy() || !ASprite) { return; }

		GameRenderer.DrawSprite(*ASprite);

		if (bDrawDebug)
		{
			GameRenderer.DrawDebugShape(ForwardVectorDebugShape());
		}
	}

	bool Actor::IsOutOfBounds(float Allowance) const
	{
		float WindowWidth = GetWindowSize().x;
		float WindowHeight = GetWindowSize().y;

		float SpriteWidth = GetSpriteBounds().size.x;
		float SpriteHeight = GetSpriteBounds().size.y;

		sf::Vector2f SpriteLocation = GetActorLocation();
		if (SpriteLocation.x < -SpriteWidth - Allowance || SpriteLocation.x > WindowWidth + SpriteWidth + Allowance || SpriteLocation.y < -SpriteHeight - Allowance || SpriteLocation.y > WindowHeight + SpriteHeight + Allowance)
		{
			return true;
		}
		return false;
	}

	void Actor::InitialziePhysics()
	{
		if (!PhysicsBody)
		{
			PhysicsBody = PhysicsSystem::Get().AddListener(this);
		}
	}

	void Actor::UninitialziePhysics()
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

	bool Actor::IsHostile(Actor* OtherActor)
	{
		if (!OtherActor)
			return false;

		const EActorID MyID = GetActorID();
		const EActorID OtherID = OtherActor->GetActorID();

		if (MyID == EActorID::Neutral || OtherID == EActorID::Neutral)
			return false;

		if (MyID == OtherID)
			return false;

		return true;
	}

	void Actor::Damage(float Amount)
	{
	}

	void Actor::SetActorLocation(const sf::Vector2f& NewLocation)
	{
		if (!ASprite) { return; }
		ASprite->setPosition(NewLocation);
		UpdatePhysicsBodyTransform();
	}

	void Actor::SetActorRotation(const sf::Angle& NewRotation)
	{
		if (!ASprite) { return; }
		ASprite->setRotation(NewRotation);
		UpdatePhysicsBodyTransform();
	}

	sf::Vector2f Actor::GetActorLocation() const
	{
		if (!ASprite) { return sf::Vector2f{}; }
		return ASprite->getPosition();
	}

	sf::Angle Actor::GetActorRotation() const
	{
		if (!ASprite) { return sf::Angle{}; }
		return ASprite->getRotation();
	}

	void Actor::AddActorLocationOffset(const sf::Vector2f& Offset)
	{
		SetActorLocation(GetActorLocation() + Offset);
	}

	void Actor::AddActorRotationOffset(const sf::Angle& RotOffset)
	{
		SetActorRotation(GetActorRotation() + RotOffset);
	}

	void Actor::SetLocalForwardVector(sf::Vector2f& Forward)
	{
		LocalForward = Normalize(Forward);
	}

	sf::Vector2f Actor::GetActorFowardVector() const
	{
		const float r = GetActorRotation().asRadians();

		const float cosR = std::cos(r);
		const float sinR = std::sin(r);

		return
		{
			LocalForward.x * cosR - LocalForward.y * sinR,
			LocalForward.x * sinR + LocalForward.y * cosR
		};
	}

	sf::Vector2f Actor::GetActorRightVector() const
	{
		return RotationToVector(GetActorRotation() + sf::degrees(90.f));
	}

	sf::VertexArray Actor::ForwardVectorDebugShape(float Length, sf::Color Color) const
	{
		const sf::Vector2f start = GetActorLocation();
		const sf::Vector2f forward = GetActorFowardVector();
		const sf::Vector2f end = start + forward * Length;

		sf::VertexArray arrow(sf::PrimitiveType::Lines);

		arrow.append({ start, Color });
		arrow.append({ end,   Color });

		constexpr float HeadSize = 10.f;
		constexpr float HeadAngle = 25.f * 3.14159265f / 180.f;

		const float angle = std::atan2(forward.y, forward.x);

		const sf::Vector2f left =
		{
			end.x - HeadSize * std::cos(angle - HeadAngle),
			end.y - HeadSize * std::sin(angle - HeadAngle)
		};

		const sf::Vector2f right =
		{
			end.x - HeadSize * std::cos(angle + HeadAngle),
			end.y - HeadSize * std::sin(angle + HeadAngle)
		};

		arrow.append({ end,  Color });
		arrow.append({ left, Color });

		arrow.append({ end,  Color });
		arrow.append({ right, Color });

		return arrow;
	}

	sf::FloatRect Actor::GetSpriteBounds() const
	{
		return ASprite->getGlobalBounds();
	}

	void Actor::SetPhysicsEnabled(bool Enabled)
	{
		bPhysicsEnabled = Enabled;
		if (bPhysicsEnabled)
		{
			InitialziePhysics();
		}
		else
		{
			UninitialziePhysics();
		}
	}

	void Actor::OnActorBeginOverlap(Actor* OtherActor)
	{
	}

	void Actor::OnActorEndOverlap(Actor* OtherActor)
	{
	}

	void Actor::CenterPivot()
	{
		if (!ASprite) { return; }
		sf::FloatRect localBounds = ASprite->getLocalBounds();
		ASprite->setOrigin({ localBounds.size.x / 2.f, localBounds.size.y / 2.f });
	}
}
#include "Framework/Actor.h"
#include "Framework/Core.h"
#include "Framework/AssetManager.h"
#include "Framework/MathUtility.h"
#include "Framework/World.h"
#include "Framework/PhysicsSystem.h"
#include "Framework/Renderer.h"

namespace we
{
	Actor::Actor(World* OwningWorld, const string& TexturePath)
		: OwningWorld{ OwningWorld },
		bHasBegunPlay{ false },
		ATexture{ nullptr },
		ASprite{ nullptr },
		bPhysicsEnabled{ false },
		APhysicsBody{ b2_nullBodyId }
	{
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

	sf::Vector2u Actor::GetWindowSize() const
	{
		return OwningWorld->GetWindowSize();
	}

	void Actor::SetTexture(const string& TexturePath, float SpriteScale)
	{
		ATexture = AssetManager::GetAssetManager().LoadTexture(TexturePath);
		if (!ATexture)
		{
			LOG("Actor: Failed to load texture: %s", TexturePath.c_str());
			return;
		}

		ASprite = shared<sf::Sprite>(new sf::Sprite(*ATexture));

		ASprite->setScale({ SpriteScale, SpriteScale });

		int TextureWidth = static_cast<int>(ATexture->getSize().x);
		int TextureHeight = static_cast<int>(ATexture->getSize().y);
		ASprite->setTextureRect(sf::IntRect({ 0, 0 }, { TextureWidth, TextureHeight }));

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
	}

	void Actor::SetPhysicsTransform(const b2Vec2& Position, const b2Rot& Rotation)
	{
		if (!ASprite) { return; }

		float InverseScale = 1.0f / PhysicsSystem::Get().GetPhysicsScale();

		ASprite->setPosition({ Position.x * InverseScale, Position.y * InverseScale });

		ASprite->setRotation(sf::radians(b2Rot_GetAngle(Rotation)));
	}

	bool Actor::IsOutOfBounds() const
	{
		float WindowWidth = GetWindowSize().x;
		float WindowHeight = GetWindowSize().y;
		float SpriteWidth = GetSpriteBounds().size.x;
		float SpriteHeight = GetSpriteBounds().size.y;
		sf::Vector2f SpriteLocation = GetActorLocation();
		if (SpriteLocation.x < -SpriteWidth || SpriteLocation.x > WindowWidth + SpriteWidth || SpriteLocation.y < -SpriteHeight || SpriteLocation.y > WindowHeight + SpriteHeight)
		{
			return true;
		}
		return false;
	}
	void Actor::SetActorLocation(const sf::Vector2f& NewLocation)
	{
		if (!ASprite) { return; }
		ASprite->setPosition(NewLocation);
		UpdatePhysicsTransforms();
	}
	void Actor::SetActorRotation(const sf::Angle& NewRotation)
	{
		if (!ASprite) { return; }
		ASprite->setRotation(NewRotation);
		UpdatePhysicsTransforms();
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
	sf::Vector2f Actor::GetActorFowardVector() const
	{
		return RotationToVector(GetActorRotation());
	}
	sf::Vector2f Actor::GetActorRightVector() const
	{
		return RotationToVector(GetActorRotation() + sf::degrees(90.f));
	}
	sf::FloatRect Actor::GetSpriteBounds() const
	{
		return ASprite->getGlobalBounds();
	}
	void Actor::SetEnablePhysics(bool Enabled)
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
	void Actor::CenterPivot()
	{
		if (!ASprite) { return; }
		sf::FloatRect localBounds = ASprite->getLocalBounds();
		ASprite->setOrigin({ localBounds.size.x / 2.f, localBounds.size.y / 2.f });
	}
	void Actor::InitializePhysics()
	{
		if (APhysicsBody.index1 == 0)
		{
			APhysicsBody = PhysicsSystem::Get().AddListener(this);
		}
	}
	void Actor::UninitializePhysics()
	{
		if (APhysicsBody.index1 != 0)
		{
			PhysicsSystem::Get().RemoveListener(APhysicsBody);
		}
	}
	void Actor::UpdatePhysicsTransforms()
	{
		if (APhysicsBody.index1 != 0)
		{
			float PhysicsScale = PhysicsSystem::Get().GetPhysicsScale();
			b2Vec2 Position{ GetActorLocation().x * PhysicsScale, GetActorLocation().y * PhysicsScale };
			b2Rot Rotation = b2MakeRot(GetActorRotation().asRadians());

			b2Body_SetTransform(APhysicsBody, Position, Rotation);
		}
	}
}
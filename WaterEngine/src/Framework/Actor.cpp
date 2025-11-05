#include "Framework/Actor.h"
#include "Framework/Core.h"
#include "Framework/AssetManager.h"
#include "Framework/MathUtility.h"

namespace we
{
	Actor::Actor(World* OwningWorld, const std::string& TexturePath)
		: OwningWorld{OwningWorld},
		bHasBegunPlay{false},
		ATexture{nullptr},
		ASprite{nullptr}
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
	void Actor::SetTexture(const std::string& TexturePath, int FrameWidth, int FrameHeight, float SpriteScale)
	{
		ATexture = AssetManager::GetAssetManager().LoadTexture(TexturePath);
		if (!ATexture)
		{
			LOG("Actor: Failed to load texture: %s", TexturePath.c_str());
			return;
		}

		ASprite = shared<sf::Sprite>(new sf::Sprite(*ATexture));

		int TextureWidth = static_cast<int>(ATexture->getSize().x);
		int TextureHeight = static_cast<int>(ATexture->getSize().y);

		FrameSize = { FrameWidth, FrameHeight };

		if (FrameWidth > 0 && FrameHeight > 0)
		{
			ASprite->setTextureRect(sf::IntRect({ 0, 0 }, { FrameWidth, FrameHeight }));
		}
		else
		{
			ASprite->setTextureRect(sf::IntRect({ 0, 0 }, { TextureWidth, TextureHeight }));
		}

		ASprite->setScale({ SpriteScale, SpriteScale });

		CenterPivot();

		LOG("Actor: Sprite created for %s", TexturePath.c_str());
	}
	void Actor::SetActorScale(float NewScale)
	{
		if (!ASprite) { return; }

		ASprite->setScale({ NewScale, NewScale });
	}
	void Actor::Render(sf::RenderWindow& Window)
	{
		if (IsPendingDestroy() || !ASprite) { return; }
		Window.draw(*ASprite);
	}
	void Actor::SetActorLocation(const sf::Vector2f& NewLocation)
	{
		ASprite->setPosition(NewLocation);
	}
	void Actor::SetActorRotation(const sf::Angle& NewRotation)
	{
		ASprite->setRotation(NewRotation);
	}
	sf::Vector2f Actor::GetActorLocation() const
	{
		return ASprite->getPosition();
	}
	sf::Angle Actor::GetActorRotation() const
	{
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
	void Actor::CenterPivot()
	{
		sf::FloatRect localBounds = ASprite->getLocalBounds();
		ASprite->setOrigin({ localBounds.size.x / 2.f, localBounds.size.y / 2.f });
	}
}
#include "Framework/Actor.h"
#include "Framework/Core.h"
#include "Framework/AssetManager.h"

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
		LOG("Actor BeginPlay Called!")
	}

	void Actor::Tick(float DeltaTime)
	{
	}
	void Actor::SetTexture(const std::string& TexturePath)
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
		ASprite->setTextureRect(sf::IntRect({ 0, 0 }, { TextureWidth, TextureHeight }));

		LOG("Actor: Sprite created for %s", TexturePath.c_str());
	}
	void Actor::Render(sf::RenderWindow& Window)
	{
		if (IsPendingDestroy() || !ASprite) { return; }
		Window.draw(*ASprite);
	}
}
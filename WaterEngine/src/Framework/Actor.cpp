
#include "Framework/Actor.h"
#include "Framework/Core.h"

namespace we
{
	Actor::Actor(World* OwningWorld, const std::string& TexturePath)
		: OwningWorld{OwningWorld},
		bHasBegunPlay{false},
		ATexture{},
		ASprite{ATexture}
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
		ATexture.loadFromFile(TexturePath);
		ASprite.setTexture(ATexture);
		int TextureWidth = ATexture.getSize().x;
		int TextureHeight = ATexture.getSize().y;
		ASprite.setTextureRect(sf::IntRect{ sf::Vector2i{}, sf::Vector2i{TextureWidth, TextureHeight} });
	}
	void Actor::Render(sf::RenderWindow& Window)
	{
		if (IsPendingDestroy()) { return; }
		Window.draw(ASprite);
	}
}
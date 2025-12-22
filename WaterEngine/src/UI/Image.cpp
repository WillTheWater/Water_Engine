#include "UI/Image.h"
#include "Framework/AssetManager.h"
#include "Framework/Renderer.h"

namespace we
{
	Image::Image(const string& TexturePath)
		: Texture{AssetManager::Get().LoadTexture(TexturePath)}
		, Sprite{*Texture}
	{
	}

	void Image::SetTexture(const shared<sf::Texture> NewTexture)
	{
		if (NewTexture)
		{
			Texture = NewTexture;
			Sprite.setTexture(*NewTexture);
		}
	}

	void Image::ScaleImage(const sf::Vector2f& NewScale)
	{
		Sprite.setScale(NewScale);
	}

	void Image::CenterOrigin()
	{
		Sprite.setOrigin({ Texture->getSize().x / 2.f, Texture->getSize().y / 2.f });
	}

	sf::FloatRect Image::GetGlobalBounds()
	{
		return Sprite.getGlobalBounds();
	}

	void Image::Render(Renderer& GameRenderer)
	{
		GameRenderer.Draw(Sprite);
	}

	void Image::UpdatePosition(const sf::Vector2f& NewPosition)
	{
		Sprite.setPosition(NewPosition);
	}

	void Image::UpdateRotation(const sf::Angle& NewRotation)
	{
		Sprite.setRotation(NewRotation);
	}
}
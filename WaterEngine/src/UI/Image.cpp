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
		Sprite.scale(NewScale);
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
#include "UI/Button.h"
#include "Framework/AssetManager.h"

namespace we
{
	Button::Button(const string& TexturePath)
		: ButtonTexture{AssetManager::Get().LoadTexture(TexturePath)}
		, ButtonSprite{*ButtonTexture}
		, DefaultColor{sf::Color::White}
		, HoverColor{sf::Color {150, 150, 150, 255}}
		, PressedColor{sf::Color {55, 55, 55,255}}
		, bIsButtonPressed{false}
	{
	}

	void Button::Render(Renderer& GameRenderer)
	{
		GameRenderer.Draw(ButtonSprite);
	}

	void Button::UpdatePosition(const sf::Vector2f& NewPosition)
	{
		ButtonSprite.setPosition(NewPosition);
	}

	void Button::UpdateRotation(const sf::Angle& NewRotation)
	{
		ButtonSprite.setRotation(NewRotation);
	}

	void Button::SetTexture(const shared<sf::Texture> NewTexture)
	{
		if (NewTexture)
		{
			ButtonTexture = NewTexture;
			ButtonSprite.setTexture(*ButtonTexture);
		}
	}

	void Button::ScaleImage(const sf::Vector2f& NewScale)
	{
		ButtonSprite.setScale(NewScale);
	}

	void Button::CenterOrigin()
	{
		ButtonSprite.setOrigin({ ButtonTexture->getSize().x / 2.f, ButtonTexture->getSize().y / 2.f });
	}

	sf::FloatRect Button::GetGlobalBounds()
	{
		return ButtonSprite.getGlobalBounds();
	}
}
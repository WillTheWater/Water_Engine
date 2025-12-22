#pragma once
#include "UI/Widget.h"
#include "Framework/Delegate.h"

namespace we
{
	class Button : public Widget
	{
	public:
		Button(const string& TexturePath = "SpaceShooterRedux/PNG/UI/buttonRed.png");

		void SetTexture(const shared<sf::Texture> NewTexture);
		void ScaleImage(const sf::Vector2f& NewScale);
		void CenterOrigin();
		sf::FloatRect GetGlobalBounds();

	protected:
		virtual void Render(Renderer& GameRenderer) override;

	private:
		virtual void UpdatePosition(const sf::Vector2f& NewPosition) override;
		virtual void UpdateRotation(const sf::Angle& NewRotation) override;
		shared<sf::Texture> ButtonTexture;
		sf::Sprite ButtonSprite;
		sf::Color DefaultColor;
		sf::Color HoverColor;
		sf::Color PressedColor;
		bool bIsButtonPressed;
	};
}
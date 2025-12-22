#pragma once
#include "sfml/Graphics.hpp"
#include "UI/Widget.h"

namespace we
{
	class Image : public Widget
	{
	public:
		Image(const string& TexturePath);

		void SetTexture(const shared<sf::Texture> NewTexture);
		void ScaleImage(const sf::Vector2f& NewScale);
		void CenterOrigin();
		sf::FloatRect GetLocalBounds();

	protected:
		virtual void Render(Renderer& GameRenderer) override;

	private:
		virtual void UpdatePosition(const sf::Vector2f& NewPosition) override;
		virtual void UpdateRotation(const sf::Angle& NewRotation) override;
		shared<sf::Texture> Texture;
		sf::Sprite Sprite;
	};
}
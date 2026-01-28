#pragma once
#include "UI/Widget.h"
#include <SFML/Graphics.hpp>

namespace we
{
	class ProgressBar : public Widget
	{
	public:
		ProgressBar(
			const sf::Vector2f& Dimensions = { 200.f, 50.f },
			float FillPercentage = 1.f
		);

		void UpdateFillPercentage(float CurrentValue, float MaxValue);
		void SetBackgroundColor(const sf::Color& NewColor);
		void SetFillBarColor(const sf::Color& NewColor);
		void SetProgressBarDimensions(const sf::Vector2f& BackgroundSize);
		sf::Vector2f GetBarSize() const { return Background.getSize(); }

	protected:
		virtual void Render(Renderer& GameRenderer) override;

	private:
		virtual void UpdatePosition(const sf::Vector2f& NewPosition) override;
		virtual void UpdateRotation(const sf::Angle& NewRotation) override;

		void CenterText();
		void UpdateFillBarPosition();

	private:
		shared<sf::Font> ProgressBarFont;
		sf::Text ProgressBarText;

		sf::RectangleShape Background;
		sf::RectangleShape FillBar;
		sf::Color BackgroundColor;
		sf::Color FillBarColor;

		float FillPercentage;
		const float Padding = 4.f;
	};
}

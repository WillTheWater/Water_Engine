#pragma once 
#include "Framework/Core.h"
#include "UI/Widget.h"

namespace we
{
	class TextBlock : public Widget
	{
	public:
		TextBlock(const string& Text, const string& FontPath = "SpaceShooterRedux/Bonus/kenvector_future.ttf", unsigned int FontSize = 40);

		void SetText(const string& InputText);
		void SetFontSize(unsigned int NewSize);

	protected:
		virtual void Render(Renderer& GameRenderer) override;
	private:
		virtual void UpdatePosition(const sf::Vector2f& NewLocation) override;
		virtual void UpdateRotation(const sf::Angle& NewRotation) override;
		shared<sf::Font> DisplayFont;
		sf::Text DisplayText;
	};
}
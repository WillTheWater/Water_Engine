#pragma once
#include "SFML/Graphics.hpp"
#include "Framework/Object.h"

namespace we
{
	class Renderer;

	class Widget : public Object
	{
	public:
		void NativeRender(Renderer& GameRenderer);
		virtual bool HandleEvent(const optional<sf::Event> Event);

		void SetWidgetLocation(const sf::Vector2f& NewLocation);
		void SetWidgetRotation(const sf::Angle& NewRotation);
		sf::Vector2f GetWidgetLocation() const { return WidgetTransforms.getPosition(); }
		sf::Angle GetWidgetRotation() const { return WidgetTransforms.getRotation(); }
		void SetVisibility(bool NewVisibility) { bIsVisible = NewVisibility; }
		bool GetVisibility() const { return bIsVisible; }

	protected:
		Widget();
		virtual void Render(Renderer& GameRenderer);

	private:
		virtual void UpdateTransforms();
		bool bIsVisible;
		sf::Transformable WidgetTransforms;
	};
}
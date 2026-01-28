#include "UI/Widget.h"

namespace we
{
	Widget::Widget()
		: bIsVisible{true}
		, WidgetTransforms{}
	{
	}


	void Widget::NativeRender(Renderer& GameRenderer)
	{
		if (bIsVisible)
		{
			Render(GameRenderer);
		}
	}

	bool Widget::HandleEvent(const optional<sf::Event> Event)
	{
		return false;
	}

	void Widget::Render(Renderer& GameRenderer)
	{

	}

	void Widget::SetWidgetPosition(const sf::Vector2f& NewPosition)
	{
		WidgetTransforms.setPosition(NewPosition);
		UpdatePosition(NewPosition);
	}

	void Widget::SetWidgetRotation(const sf::Angle& NewRotation)
	{
		WidgetTransforms.setRotation(NewRotation);
		UpdateRotation(NewRotation);
	}

	void Widget::UpdatePosition(const sf::Vector2f& NewPosition)
	{
	}

	void Widget::UpdateRotation(const sf::Angle& NewRotation)
	{
	}
}
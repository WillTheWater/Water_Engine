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

	void Widget::Render(Renderer& GameRenderer)
	{

	}

	void Widget::SetWidgetLocation(const sf::Vector2f& NewLocation)
	{
		WidgetTransforms.setPosition(NewLocation);
		UpdateTransforms();
	}

	void Widget::SetWidgetRotation(const sf::Angle& NewRotation)
	{
		WidgetTransforms.setRotation(NewRotation);
		UpdateTransforms();
	}

	void Widget::UpdateTransforms()
	{
	}
}
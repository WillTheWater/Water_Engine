#include "Framework/Renderer.h"

namespace we
{
	Renderer::Renderer(sf::RenderWindow& Window)
		: Window{ Window }
	{
	}

	void Renderer::Clear()
	{
		Window.clear();
	}

	void Renderer::Display()
	{
		Window.display();
	}
}
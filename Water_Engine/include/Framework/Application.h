#pragma once

#include <SFML/Graphics.hpp>

namespace we
{
	class Application
	{
	public:
		Application();
		void Run();

	private:
		sf::RenderWindow Window;
	};
}
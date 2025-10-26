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
		void Tick(float DeltaTime);
		void Render();

		sf::RenderWindow Window;

		float TargetFramerate;

		sf::Clock TickClock;
	};
}
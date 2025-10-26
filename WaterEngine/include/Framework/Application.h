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
		void TickGlobal(float DeltaTime);
		void Renderer();

		virtual void Render();
		virtual void Tick(float DeltaTime);

		sf::RenderWindow Window;

		float TargetFramerate;

		sf::Clock TickClock;
	};
}
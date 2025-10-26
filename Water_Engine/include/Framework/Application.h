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
		virtual void Render() = 0;
		virtual void Tick(float DeltaTime) = 0;

		sf::RenderWindow Window;

		float TargetFramerate;

		sf::Clock TickClock;
	};
}
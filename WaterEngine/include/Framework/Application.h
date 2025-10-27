#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Core.h"

namespace we
{
	class World;

	class Application
	{
	public:
		Application();
		void Run();

		template<typename WorldType>
		weak<WorldType> LoadWorld();


	private:
		void TickGlobal(float DeltaTime);
		void Renderer();

		virtual void Render();
		virtual void Tick(float DeltaTime);

		sf::RenderWindow Window;
		float TargetFramerate;
		sf::Clock TickClock;

		shared<World> CurrentWorld;
	};

	template<typename WorldType>
	weak<WorldType> Application::LoadWorld()
	{
		shared< WorldType> NewWorld{ new WorldType{this} };
		CurrentWorld = NewWorld;
		CurrentWorld->BeginPlayGlobal();
		return NewWorld;
	}
}
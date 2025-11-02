#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Core.h"
#include <stdint.h>

namespace we
{
	class World;

	class Application
	{
	public:
		Application(unsigned int WindowWidth, unsigned int WindowHeight, const std::string& WindowTitle, std::uint32_t WindowStyle);
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
		sf::Clock GarbageCollectionClock;
		float CollectionInterval;

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
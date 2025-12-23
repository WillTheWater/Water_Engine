#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Core.h"
#include <stdint.h>

namespace we
{
	class World;
	class Renderer;

	class Application
	{
	public:
		Application(unsigned int WindowWidth, unsigned int WindowHeight, const std::string& WindowTitle, std::uint32_t WindowStyle);
		~Application();

		void Run();
		void QuitApplication();

		template<typename WorldType>
		weak<WorldType> LoadWorld();

		sf::Vector2u GetWindowSize() const;

	private:
		void TickGlobal(float DeltaTime);
		void RendererCycle();
		bool DispatchEvent(const optional<sf::Event> Event);

		virtual void Render(Renderer& GameRenderer);
		virtual void Tick(float DeltaTime);

		sf::RenderWindow Window;
		unique<Renderer> GameRenderer;

		float TargetFramerate;
		sf::Clock TickClock;
		sf::Clock GarbageCollectionClock;
		float CollectionInterval;

		shared<World> CurrentWorld;
		shared<World> PendingWorld;
	};

	template<typename WorldType>
	weak<WorldType> Application::LoadWorld()
	{
		shared< WorldType> NewWorld{ new WorldType{this} };
		PendingWorld = NewWorld;
		return NewWorld;
	}
}
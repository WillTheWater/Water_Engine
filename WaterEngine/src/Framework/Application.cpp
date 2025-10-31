#include "Framework/Application.h"
#include "Framework/Core.h"
#include "Framework/World.h"

namespace we
{
	Application::Application(unsigned int WindowWidth, unsigned int WindowHeight, const std::string& WindowTitle, std::uint32_t WindowStyle)
		: Window{ sf::VideoMode({ WindowWidth, WindowHeight }), WindowTitle, WindowStyle },
		TargetFramerate{ 60.f },
		TickClock{},
		CurrentWorld{ nullptr }
	{
	}

	void Application::Run()
	{
		TickClock.restart();
		float AccumulatedTime = 0.f;
		float TargetDeltaTime = 1.f / TargetFramerate;

        while (Window.isOpen())
        {
            while (const std::optional event = Window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    Window.close();
            }
			float FrameTick = TickClock.restart().asSeconds();
			AccumulatedTime += FrameTick;
			while (AccumulatedTime > TargetDeltaTime)
			{
				AccumulatedTime -= TargetDeltaTime;
				TickGlobal(TargetDeltaTime);
				Renderer();
			}
        }
	}
	void Application::TickGlobal(float DeltaTime)
	{
		Tick(DeltaTime);

		if (CurrentWorld)
		{
			CurrentWorld->TickGlobal(DeltaTime);
		}
	}
	void Application::Renderer()
	{
		Window.clear();
		Render();
		Window.display();
	}
	void Application::Render()
	{
	}
	void Application::Tick(float DeltaTime)
	{
	}
}
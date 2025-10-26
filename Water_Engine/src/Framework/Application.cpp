#include "Framework/Application.h"
#include <iostream>

namespace we
{
	Application::Application()
		: Window{ sf::VideoMode({ 1920, 1080 }), "Game Window" },
		TargetFramerate{60.f},
		TickClock{}
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
			AccumulatedTime += TickClock.restart().asSeconds();
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
	}
	void Application::Renderer()
	{
		Window.clear();
		Render();
		Window.display();
	}
}
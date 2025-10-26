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
        }
		AccumulatedTime += TickClock.restart().asSeconds();
		while (AccumulatedTime > TargetDeltaTime)
		{
			AccumulatedTime -= TargetDeltaTime;
			Tick(TargetDeltaTime);
			Render();
		}
	}
	void Application::Tick(float DeltaTime)
	{
		std::cout << "FPS: " << 1.f / DeltaTime << std::endl;
	}
	void Application::Render()
	{
	}
}
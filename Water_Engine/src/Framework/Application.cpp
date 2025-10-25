#include "Framework/Application.h"

namespace we
{
	Application::Application()
		: Window{ sf::VideoMode({ 1920, 1080 }), "Game Window" }
	{
		
	}
	void Application::Run()
	{
        while (Window.isOpen())
        {
            while (const std::optional event = Window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    Window.close();
            }

            Window.clear();
            Window.display();
        }
	}
}
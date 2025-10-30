﻿// Copyright (C) 2025 Stoic Ronin Studio. All Rights Reserved.

#include "Framework/Application.h"
#include "Framework/Core.h"
#include "Framework/World.h"

namespace we
{
	Application::Application()
		: Window{ sf::VideoMode({ 1920, 1080 }), "Game Window" },
		TargetFramerate{60.f},
		TickClock{},
		CurrentWorld{nullptr}
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
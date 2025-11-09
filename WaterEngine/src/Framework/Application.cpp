#include "Framework/Application.h"
#include "Framework/Core.h"
#include "Framework/World.h"
#include "Framework/AssetManager.h"
#include "Framework/PhysicsSystem.h"

namespace we
{
	Application::Application(unsigned int WindowWidth, unsigned int WindowHeight, const std::string& WindowTitle, std::uint32_t WindowStyle)
		: Window{ sf::VideoMode({ WindowWidth, WindowHeight }), WindowTitle, WindowStyle },
		TargetFramerate{ 60.f },
		TickClock{},
		GarbageCollectionClock{},
		CollectionInterval{2.f},
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

		PhysicsSystem::GetPhysiscSystem().Step(DeltaTime);

		if (GarbageCollectionClock.getElapsedTime().asSeconds() >= CollectionInterval)
		{
			GarbageCollectionClock.restart();
			AssetManager::GetAssetManager().GarbageCollectionCycle();
			if (CurrentWorld)
			{
				CurrentWorld->GarbageCollectionCycle();
			}
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
		if (CurrentWorld)
		{
			CurrentWorld->Render(Window);
		}
	}
	void Application::Tick(float DeltaTime)
	{
	}
}
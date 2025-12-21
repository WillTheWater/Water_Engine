#include "Framework/Application.h"
#include "Framework/Core.h"
#include "Framework/World.h"
#include "Framework/AssetManager.h"
#include "Framework/PhysicsSystem.h"
#include "Framework/Renderer.h"
#include "Framework/TimerManager.h"

namespace we
{
	Application::Application(unsigned int WindowWidth, unsigned int WindowHeight, const std::string& WindowTitle, std::uint32_t WindowStyle)
		: Window{ sf::VideoMode({ WindowWidth, WindowHeight }), WindowTitle, WindowStyle }
		, TargetFramerate{ 60.f } 
		, TickClock{}
		, GarbageCollectionClock{} 
		, CollectionInterval{ 2.f }
		, CurrentWorld{ nullptr }
		, GameRenderer{ std::make_unique<Renderer>(Window) }
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		TickClock.restart();
		float AccumulatedTime = 0.f;
		float TargetDeltaTime = 1.f / TargetFramerate;

		while (Window.isOpen())
		{
			while (const std::optional Event = Window.pollEvent())
			{
				if (Event->is<sf::Event::Closed>())
				{
					Window.close();
				}
				else
				{
					BroadcastEvent(Event);
				}
			}
			float FrameTick = TickClock.restart().asSeconds();
			AccumulatedTime += FrameTick;
			while (AccumulatedTime > TargetDeltaTime)
			{
				AccumulatedTime -= TargetDeltaTime;
				TickGlobal(TargetDeltaTime);
			}
			RendererCycle();
		}
	}

	void Application::TickGlobal(float DeltaTime)
	{
		Tick(DeltaTime);

		if (CurrentWorld)
		{
			CurrentWorld->TickGlobal(DeltaTime);
		}

		TimerManager::Get().UpdateTimer(DeltaTime);
		PhysicsSystem::Get().Step(DeltaTime);

		if (GarbageCollectionClock.getElapsedTime().asSeconds() >= CollectionInterval)
		{
			GarbageCollectionClock.restart();
			AssetManager::Get().GarbageCollectionCycle();
			if (CurrentWorld)
			{
				CurrentWorld->GarbageCollectionCycle();
			}
		}
	}

	void Application::RendererCycle()
	{
		if (!GameRenderer) return;

		GameRenderer->Clear();
		Render(*GameRenderer);
		GameRenderer->Display();
	}

	bool Application::DispatchEvent(const optional<sf::Event> Event)
	{
		if (CurrentWorld)
		{
			return CurrentWorld->DispatchEvent(Event);
		}
		return false;
	}

	void Application::Render(Renderer& GameRenderer)
	{
		if (CurrentWorld)
		{
			CurrentWorld->Render(GameRenderer);
		}
	}

	void Application::Tick(float DeltaTime)
	{
	}
}
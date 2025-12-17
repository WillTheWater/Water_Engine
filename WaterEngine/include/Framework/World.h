#pragma once
#include "Framework/Core.h"
#include "SFML/Graphics.hpp"
#include "Framework/Object.h"

namespace we
{
	class Application;
	class Actor;
	class Renderer;

	class World : public Object
	{
	public:
		World(Application* OwningApp);
		virtual ~World();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);

		void Render(Renderer& GameRenderer);

		void GarbageCollectionCycle();

		template<typename ActorType, typename... Args>
		weak<ActorType> SpawnActor(Args... args);

		sf::Vector2u GetWindowSize() const;

	private:
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		Application* OwningApp;
		bool bHasBegunPlay;

		List<shared<Actor>> Actors;
		List<shared<Actor>> PendingActors;
	};

	template<typename ActorType, typename... Args>
	weak<ActorType> World::SpawnActor(Args... args)
	{
		shared<ActorType> NewActor{ new ActorType(this, args...) };
		PendingActors.push_back(NewActor);
		return NewActor;
	}
}
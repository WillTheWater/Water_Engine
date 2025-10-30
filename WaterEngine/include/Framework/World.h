#pragma once
#include "Framework/Core.h"

namespace we
{
	class Application;
	class Actor;

	class World
	{
	public:
		World(Application* OwningApp);
		virtual ~World();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);

		template<typename ActorType>
		weak<Actor> SpawnActor();

	private:
		void BeginPlay();
		void Tick(float DeltaTime);

		Application* OwningApp;
		bool bHasBegunPlay;

		List<shared<Actor>> Actors;
		List<shared<Actor>> PendingActors;
	};

	template<typename ActorType>
	weak<Actor> World::SpawnActor()
	{
		shared<Actor> NewActor{ new ActorType{this} };
		PendingActors.push_back(NewActor);
		return NewActor;
	}
}
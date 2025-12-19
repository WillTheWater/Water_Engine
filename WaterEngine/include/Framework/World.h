#pragma once
#include "Framework/Core.h"
#include "SFML/Graphics.hpp"
#include "Framework/Object.h"

namespace we
{
	class Application;
	class Actor;
	class Renderer;
	class Level;

	class World : public Object
	{
	public:
		World(Application* OwningApp);
		virtual ~World();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);

		void Render(Renderer& GameRenderer);

		void GarbageCollectionCycle();

		void AddLevel(const shared<Level>& NewLevel);

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
		List<shared<Level>> Levels;
		List<shared<Level>>::iterator CurrentLevel;
		virtual void InitLevels();
		virtual void EndLevels();
		void LoadNextLevel();
		void StartLevels();
	};

	template<typename ActorType, typename... Args>
	weak<ActorType> World::SpawnActor(Args... args)
	{
		shared<ActorType> NewActor{ new ActorType(this, args...) };
		PendingActors.push_back(NewActor);
		return NewActor;
	}
}
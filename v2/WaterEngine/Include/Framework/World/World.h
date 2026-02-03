// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Framework/World/Object/Object.h"

namespace we
{
	class Actor;
	struct EngineSubsystem;

	class World
	{
	public:
		explicit World(EngineSubsystem& Subsystem);
		virtual ~World();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);
		void Render();


	public:
		template<typename ActorType>
		weak<ActorType> SpawnActor();

		list<shared<Actor>> GetActors() const { return Actors; }

		EngineSubsystem& GetSubsystem() const { return Subsystem; }

	private:
		EngineSubsystem& Subsystem;
		list<shared<Actor>> Actors;
		list<shared<Actor>> PendingActors;
		bool bHasBegunPlay;

	private:
		void BeginPlay();
		void Tick(float DeltaTime);
		void ManageActors(float DeltaTime);
	};

	template<typename ActorType>
	inline weak<ActorType> World::SpawnActor()
	{
		shared<ActorType> NewActor{ new ActorType{this, "Assets/Icon/icon.png"}};
		PendingActors.push_back(NewActor);
		return NewActor;
	}
}
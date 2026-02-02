// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
	class Actor;
	struct EngineSubsystem;

	class World
	{
	public:
		explicit World(EngineSubsystem& Subsystem);
		~World();

		template<typename ActorType, typename... Args>
		weak<ActorType> SpawnActor(Args... args);

		void Tick(float DeltaTime);
		void Render();

	private:
		EngineSubsystem& Subsystem;
		list<shared<Actor>> Actors;
		list<shared<Actor>> PendingActors;

	private:
		void FlushActors();
	};

	template<typename ActorType, typename ...Args>
	inline weak<ActorType> World::SpawnActor(Args ...args)
	{
		shared<ActorType> NewActor{ new ActorType(args...) };
		PendingActors.push_back(NewActor);
		return NewActor;
	}
}
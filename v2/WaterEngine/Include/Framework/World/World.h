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

		template<typename ActorType, typename... Args>
		weak<ActorType> SpawnActor(Args... args);

		void BeginPlay();
		void Tick(float DeltaTime);
		void Render();

		list<shared<Actor>> GetActors() const { return Actors; }

	private:
		EngineSubsystem& Subsystem;
		list<shared<Actor>> Actors;
		list<shared<Actor>> PendingActors;
		bool bHasBegunPlay;

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
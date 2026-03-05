// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object.h"
#include "Subsystem/WorldSubsystem.h"

namespace we
{
	class Actor;

	class World : public Object
	{
	public:
		World(WorldSubsystem& Subsystem);
		virtual ~World();

		void StartPlay();
		void StartTick(float DeltaTime);
		void EndPlay();
		void GarbageCollection();

		template<typename ActorType, typename... Args>
		weak<ActorType> SpawnActor(Args&&... args);

		template<typename WorldType>
		void LoadWorld() { Subsystem.CreateWorld<WorldType>(); }

	protected:
		WorldSubsystem& Subsystem;

		virtual void PreConstruct() {}
		virtual void BeginPlay() {}
		virtual void Tick(float DeltaTime) {}

	private:
		bool bHasBegunPlay;
		vector<shared<Actor>> PendingActors;
		vector<shared<Actor>> Actors;
	};

	template<typename ActorType, typename... Args>
	inline weak<ActorType> World::SpawnActor(Args&&... args)
	{
		shared<ActorType> NewActor = make_shared<ActorType>(this, std::forward<Args>(args)...);
		PendingActors.push_back(NewActor);
		return NewActor;
	}
}
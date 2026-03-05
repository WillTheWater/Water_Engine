// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object.h"

namespace we
{
	class Actor;

	class World : public Object
	{
	public:
		World();
		virtual ~World();

		void StartPlay();
		void StartTick(float DeltaTime);

		virtual void PreConstruct();
		virtual void BeginPlay();
		virtual void EndPlay();
		virtual void Tick(float DeltaTime);

		template<typename ActorType, typename... Args>
		weak<ActorType> SpawnActor(Args&&... args);

	private:
		void GarbageCollection();

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
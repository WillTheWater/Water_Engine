// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	struct EngineSubsystem;
	class Actor;

	// =============================================================================
	// WORLD - Container for all game objects in a level
	// =============================================================================

	class World
	{
	public:
		explicit World(EngineSubsystem& Subsystem);
		virtual ~World();

		// Lifecycle
		virtual void Construct();
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);

		// Actor spawning
		template<typename ActorType, typename... Args>
		weak<ActorType> SpawnActor(Args&&... args);

		// Access
		EngineSubsystem& GetSubsystem() const { return Subsystem; }
		const vector<shared<Actor>>& GetActors() const { return Actors; }

	protected:
		EngineSubsystem& Subsystem;
		vector<shared<Actor>> Actors;
		vector<shared<Actor>> PendingActors;
		bool bHasBegunPlay = false;

	private:
		void ManageActors(float DeltaTime);
	};

	template<typename ActorType, typename... Args>
	inline weak<ActorType> World::SpawnActor(Args&&... args)
	{
		static_assert(std::is_base_of_v<Actor, ActorType>, "SpawnActor only accepts Actor-derived types");
		shared<ActorType> NewActor = make_shared<ActorType>(this, std::forward<Args>(args)...);
		PendingActors.push_back(NewActor);
		return NewActor;
	}
}

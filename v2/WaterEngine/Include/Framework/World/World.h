// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object.h"
#include "Framework/World/Actor.h"
#include "Subsystem/WorldSubsystem.h"

namespace we
{
	class PhysicsSubsystem;
	class CameraSubsystem;
	class SaveSubsystem;
	class GameInstance;

	class World : public Object
	{
	public:
		World(WorldSubsystem& Subsystem);
		virtual ~World();

		void StartPlay();
		void StartTick(float DeltaTime);
		void EndingPlay();
		void GarbageCollection();

		template<typename ActorType, typename... Args>
		weak<ActorType> SpawnActor(Args&&... args);

		template<typename Func>
		void OrderActorDrawables(Func&& Callback);

		template<typename WorldType>
		void LoadWorld() { Subsystem.LoadWorld<WorldType>(); }

		const vector<shared<Actor>>& GetActors() const { return Actors; }
		
		PhysicsSubsystem& GetPhysics() { return Subsystem.GetPhysics(); }
		CameraSubsystem& GetCamera() { return Subsystem.GetCamera(); }
		SaveSubsystem& GetSave() { return Subsystem.GetSave(); }
		GameInstance& GetGameInstance() { return Subsystem.GetGameInstance(); }

	protected:
		WorldSubsystem& Subsystem;

		virtual void PreConstruct() {}
		virtual void BeginPlay() {}
		virtual void Tick(float DeltaTime) {}
		virtual void EndPlay() {}

	private:
		bool bHasBegunPlay;
		vector<shared<Actor>> PendingActors;
		vector<shared<Actor>> Actors;
	};

	template<typename ActorType, typename... Args>
	inline weak<ActorType> World::SpawnActor(Args&&... args)
	{
		auto NewActor = make_shared<ActorType>(*this, std::forward<Args>(args)...);
		PendingActors.push_back(NewActor);
		return NewActor;
	}

	template<typename Func>
	inline void World::OrderActorDrawables(Func&& Callback)
	{
		static vector<pair<uint32_t, float>> Indices;
		Indices.clear();
		Indices.reserve(Actors.size());

		for (uint32_t i = 0; i < Actors.size(); ++i)
		{
			const auto& Actor = Actors[i];
			if (Actor->IsVisible() && Actor->GetDrawable())
			{
				Indices.emplace_back(i, Actor->GetRenderDepth());
			}
		}

		std::sort(Indices.begin(), Indices.end(),
			[](const auto& A, const auto& B) { return A.second < B.second; });

		for (const auto& [Index, Depth] : Indices)
		{
			std::forward<Func>(Callback)(Actors[Index]->GetDrawable());
		}
	}
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object.h"
#include "Framework/World/RenderTypes.h"

namespace we
{
	class Actor;
	struct EngineSubsystem;

	class World : public Object
	{
	public:
		World(EngineSubsystem& InSubsystem);
		virtual ~World();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);
		void CollectRenderDepths(vector<RenderDepth>& OutDepths);

		// Dirty flag for render sorting
		void MarkRenderDirty() { bRenderOrderDirty = true; }
		bool IsRenderDirty() const { return bRenderOrderDirty; }

		// Lifecycle
		virtual void Construct();
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		// Actor spawning
		template<typename ActorType, typename... Args>
		weak<ActorType> SpawnActor(Args&&... args);

		EngineSubsystem& GetSubsystem() const { return Subsystem; }

	protected:
		void AddRenderDepth(const drawable* Drawable, float Depth);
		void ClearManualRenderDepths();

	private:
		void ManageActors(float DeltaTime);

	private:
		EngineSubsystem& Subsystem;
		vector<shared<Actor>> Actors;
		vector<shared<Actor>> PendingActors;
		bool bHasBegunPlay = false;
		vector<RenderDepth> ManualRenderDepths;
		vector<RenderDepth> CachedRenderDepths;
		bool bRenderOrderDirty = true;
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

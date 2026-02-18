// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object/Object.h"
#include "Framework/World/RenderTypes.h"
#include "Framework/World/BoundarySystem.h"

namespace we
{
	class Actor;
	struct EngineSubsystem;

	class World : public Object
	{
	public:
		World(EngineSubsystem& Subsystem);
		virtual ~World();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);
		void CollectRenderDepths(vector<RenderDepth>& OutDepths);

		// Dirty flag - set to true when actors are added/removed/moved to trigger re-sort
		void MarkRenderDirty() { bRenderOrderDirty = true; }
		bool IsRenderDirty() const { return bRenderOrderDirty; }

		virtual void Construct();
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		template<typename ActorType, typename... Args>
		weak<ActorType> SpawnActor(Args&&... args);

		EngineSubsystem& GetSubsystem() const { return Subsystem; }

		// Boundaries
		BoundarySystem& GetBoundaries() { return Boundaries; }
		const BoundarySystem& GetBoundaries() const { return Boundaries; }

	protected:
		EngineSubsystem& Subsystem;

	protected:
		void AddRenderDepth(const drawable* Drawable, float Depth);
		void ClearManualRenderDepths();

	private:
		void ManageActors(float DeltaTime);

	private:
		vector<shared<Actor>> Actors;
		vector<shared<Actor>> PendingActors;
		bool bHasBegunPlay;
		vector<RenderDepth> ManualRenderDepths;
		vector<RenderDepth> CachedRenderDepths;  // Cached sorted drawables
		bool bRenderOrderDirty = true;  // Force initial sort
		BoundarySystem Boundaries;
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
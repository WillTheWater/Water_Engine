// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"
#include "Interface/Physics/IPhysicsContactListener.h"
#include "Utility/Delegate.h"

class b2Body;

namespace we
{
	class CollisionComponent : public IActorComponent, public IPhysicsContactListener
	{
	public:
		explicit CollisionComponent(Actor* InOwner);
		~CollisionComponent();

		void SetRadius(float RadiusPixels);
		
		// Shape offset from actor center (in pixels)
		void SetShapeOffset(vec2f Offset);
		vec2f GetShapeOffset() const { return ShapeOffset; }

		// IActorComponent
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		Actor* GetOwner() const override;

		// IPhysicsContactListener
		void OnComponentBeginOverlap(ActorID OtherID) override;
		void OnComponentEndOverlap(ActorID OtherID) override;
		void SetCollisionChannel(ECollisionChannel Channel) override;

		// Overlap queries
		bool IsOverlapping() const { return !OverlappingIDs.empty(); }
		bool IsOtherActor(Actor* CheckActor) const;

		// Delegates
		Delegate<Actor*> OnBeginOverlap;
		Delegate<Actor*> OnEndOverlap;
		
		// Overlapping actors, resolved to live pointers at call time. IDs whose
		// actor no longer exists are skipped, so callers never see a dangling
		// pointer.
		vector<Actor*> GetOtherActors() const;

		// Get first overlapping actor of type
		template<typename T>
		T* GetOtherActor() const
		{
			for (Actor* Other : GetOtherActors())
			{
				if (auto* Casted = dynamic_cast<T*>(Other))
					return Casted;
			}
			return nullptr;
		}

		// Get all overlapping actors of type
		template<typename T>
		vector<T*> GetOtherActorsOfType() const
		{
			vector<T*> Result;
			for (Actor* Other : GetOtherActors())
			{
				if (auto* Casted = dynamic_cast<T*>(Other))
					Result.push_back(Casted);
			}
			return Result;
		}

		int GetOverlapCount() const { return static_cast<int>(OverlappingIDs.size()); }

		const drawable* DrawDebug();
		bool IsDebugDrawEnabled() const { return bDebugDrawEnabled; }

	private:
		void CreateBody();
		void DestroyBody();

	private:
		Actor* Owner;
		float Radius = 32.0f;
		b2Body* Body = nullptr;
		vec2f ShapeOffset{0.0f, 0.0f};
		ECollisionChannel CollisionChannel = ECollisionChannel::Interaction;
		bool bDebugDrawEnabled = false;
		
		set<ActorID> OverlappingIDs;
		optional<circle> DebugCircle;
	};
}
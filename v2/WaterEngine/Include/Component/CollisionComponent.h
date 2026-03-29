// =============================================================================
// Water Engine v2.0.0
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

		void SetRadius(float RadiusPixels);

		// IActorComponent
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		Actor* GetOwner() const override;

		// IPhysicsContactListener
		void OnComponentBeginOverlap(b2Body* OtherBody) override;
		void OnComponentEndOverlap(b2Body* OtherBody) override;
		void SetCollisionChannel(ECollisionChannel Channel) override;

		// Overlap queries
		bool IsOverlapping() const { return !OverlappingActors.empty(); }
		bool IsOtherActor(Actor* CheckActor) const;

		// Delegates
		Delegate<Actor*> OnBeginOverlap;
		Delegate<Actor*> OnEndOverlap;
		
		// Get all overlapping actors
		const set<Actor*>& GetOtherActors() const { return OverlappingActors; }
		
		// Get first overlapping actor of type
		template<typename T>
		T* GetOtherActor() const
		{
			for (Actor* Other : OverlappingActors)
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
			for (Actor* Other : OverlappingActors)
			{
				if (auto* Casted = dynamic_cast<T*>(Other))
					Result.push_back(Casted);
			}
			return Result;
		}
		
		int GetOverlapCount() const { return static_cast<int>(OverlappingActors.size()); }

		const drawable* DrawDebug();

	private:
		void CreateBody();
		void DestroyBody();
		Actor* GetActorFromBody(b2Body* Body) const;

	private:
		Actor* Owner;
		float Radius = 32.0f;
		b2Body* Body = nullptr;
		ECollisionChannel CollisionChannel = ECollisionChannel::Interaction;
		
		set<Actor*> OverlappingActors;
		optional<circle> DebugCircle;
	};
}
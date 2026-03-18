// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"
#include "Interface/Physics/IPhysicsContactListener.h"

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
		void OnBeginOverlap(b2Body* OtherBody) override;
		void OnEndOverlap(b2Body* OtherBody) override;

		// Queries
		bool IsOverlapping() const { return OverlapCount > 0; }
		int GetOverlapCount() const { return OverlapCount; }

		// Debug
		const drawable* DrawDebug();

	private:
		void CreateBody();
		void DestroyBody();

	private:
		Actor* Owner;
		float Radius = 32.0f;
		b2Body* Body = nullptr;
		int OverlapCount = 0;
		
		optional<circle> DebugCircle;
	};
}
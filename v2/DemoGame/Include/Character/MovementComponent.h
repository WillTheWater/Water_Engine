// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"
#include "Interface/Component/IActorComponent.h"

namespace we
{
	class MovementComponent : public IActorComponent
	{
	public:
		MovementComponent(Actor* InOwner);

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void EndPlay() override;
		virtual Actor* GetOwner() const override;

		void AddInputVector(const vec2f& Direction);
		void SetSpeed(float InSpeed);

	private:
		Actor* Owner;
		float Speed = 200.0f;
		vec2f Velocity;
		vec2f InputVector;
	};
}
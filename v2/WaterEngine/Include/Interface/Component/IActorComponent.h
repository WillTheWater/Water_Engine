// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"

namespace we
{
	class Actor;

	class IActorComponent
	{
	public:
		virtual ~IActorComponent() = default;

		virtual void BeginPlay() = 0;
		virtual void Tick(float DeltaTime) = 0;
		virtual void EndPlay() = 0;

		virtual Actor* GetOwner() const = 0;
	};
}
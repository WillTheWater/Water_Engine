// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object.h"

namespace we
{
	class World;

	class Actor : public Object
	{
	public:
		Actor(World* OwningWorld);
		virtual ~Actor();

		void StartPlay();
		void StartTick(float DeltaTime);

		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		bool HasBegunPlay() const { return bHasBegunPlay; }
		void SetVisible(bool bVisible) { bIsVisible = bVisible; }
		bool IsVisible() const { return bIsVisible; }

		World* GetWorld() const { return OwningWorld; }

	private:
		World* OwningWorld;

		bool bIsVisible;
		bool bHasBegunPlay;
	};
}
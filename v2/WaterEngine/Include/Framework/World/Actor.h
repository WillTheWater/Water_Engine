// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	class World;

	// =============================================================================
	// ACTOR - Base class for all game objects
	// =============================================================================

	class Actor
	{
	public:
		Actor(World* InWorld);
		virtual ~Actor();

		// Lifecycle
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);
		virtual void Destroy();

		// State
		bool HasBegunPlay() const { return bHasBegunPlay; }
		bool IsPendingDestroy() const { return bPendingDestroy; }
		bool IsVisible() const { return bVisible; }

		// Transform
		void SetPosition(const vec2f& Pos);
		vec2f GetPosition() const { return Position; }

		// Rendering
		virtual const drawable* GetDrawable() const { return nullptr; }
		virtual float GetRenderDepth() const { return Position.y; }

	protected:
		World* OwnerWorld;
		vec2f Position;
		bool bHasBegunPlay = false;
		bool bPendingDestroy = false;
		bool bVisible = true;
	};
}

// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	class RenderSubsystem;

	class CursorSubsystem
	{
	public:
		CursorSubsystem();

		void Update(float DeltaTime);
		void Render(RenderSubsystem& Renderer) const;

		void SetSpeed(float Speed) { CursorSpeed = Speed; }
		float GetSpeed() const { return CursorSpeed; }

		void SetVisibility(bool Visible) { bIsVisible = Visible; }
		bool IsVisible() const { return bIsVisible; }

		void SetPosition(vec2f Position);
		vec2f GetPosition() const;

	private:
		void InitializeCursor();

	private:
		rectangle CursorShape;
		shared<texture> CursorTexture; // Reserved for later
		float CursorSpeed;
		bool bIsVisible;
	};
}
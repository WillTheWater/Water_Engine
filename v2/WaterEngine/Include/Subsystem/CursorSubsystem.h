// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Subsystem/ResourceSubsystem.h"

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

		void SetCursorSize(vec2f Size);
		vec2f GetCursorSize() const { return CursorSize; }

		void SetPosition(vec2f Position);
		vec2f GetPosition() const;
		
		// Raw pixel position for UI hit testing
		void SetPixelPosition(vec2f PixelPos) { PixelPosition = PixelPos; }
		vec2f GetPixelPosition() const { return PixelPosition; }

	private:
		void ApplyCursorSize();

	private:
		texture CursorTexture;
		sprite CursorSprite;
		vec2f CursorSize;
		float CursorSpeed;
		bool bIsVisible;
		vec2f PixelPosition;  // Raw pixel position for UI
	};
}
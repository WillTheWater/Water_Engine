// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"

namespace we
{
	class ResourceSubsystem;

	class CursorSubsytem
	{
	public:
		explicit CursorSubsytem(const EngineConfig::CursorConfig& Config, ResourceSubsystem& Resources);

		void Update(float DeltaTime);

		void SetSpeed(float Speed);
		float GetSpeed() const { return CursorSpeed; }

		void SetVisibility(bool Visible);
		bool IsVisible() const { return bIsVisible; }

		void SetCursorSize(vec2f Size);
		vec2f GetCursorSize() const { return CursorSize; }

		void SetPosition(vec2f Position);
		vec2f GetPosition() const;

		void UpdateFromMouse(vec2f MousePosition);

		// Drawable access for rendering
		const drawable* GetDrawable() const;

		// Visual style
		void SetShapeRadius(float Radius);
		void SetShapeColor(const color& Color);

		// Sprite mode
		void SetTexture();
		bool HasTexture() const;

	private:
		void ApplyCursorSize();
		void CenterCursor();

	private:
		EngineConfig::CursorConfig Config;
		ResourceSubsystem& Resources;

		// Default circle shape (fallback when no texture)
		circle DefaultShape;

		// Optional sprite mode
		optional<sprite> CursorSprite;
		shared<texture> CursorTexture;

		// Current position
		vec2f CurrentPosition;

		// Mode tracking
		enum class CursorMode { Shape, Sprite };
		CursorMode Mode = CursorMode::Shape;

		vec2f CursorSize;
		float CursorSpeed;
		bool bIsVisible;
	};
}

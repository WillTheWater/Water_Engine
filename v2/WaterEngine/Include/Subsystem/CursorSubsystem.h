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
	class WindowSubsystem;
	class CameraSubsystem;

	struct CursorConfig
	{
		stringView DefaultCursor;
		vec2f DefaultCursorSize;
		float DefaultCursorSpeed;
		float JoystickDeadzone;
		class WindowSubsystem& Window;
	};

	class CursorSubsystem
	{
	public:
		explicit CursorSubsystem(const CursorConfig& Config);

		void Update(float DeltaTime);
		void Render(RenderSubsystem& Renderer) const;

		void SetSpeed(float Speed) { CursorSpeed = Speed; }
		float GetSpeed() const { return CursorSpeed; }

		void SetVisibility(bool Visible);
		bool IsVisible() const { return bIsVisible; }

		void SetCursorSize(vec2f Size);
		vec2f GetCursorSize() const { return CursorSize; }

		// Cursor position in window pixel coordinates
		void SetPosition(vec2f Position);
		vec2f GetPosition() const;

		// Convert window pixel position to world coordinates using camera
		vec2f GetWorldPosition(const CameraSubsystem& Camera) const;

		void SetJoystickDeadzone(float Deadzone) { Config.JoystickDeadzone = Deadzone; }
		float GetJoystickDeadzone() const { return Config.JoystickDeadzone; }

	private:
		void ApplyCursorSize();
		void CenterCursor();
		vec2f GetWindowSize() const;

	private:
		texture CursorTexture;
		sprite CursorSprite;
		vec2f CursorSize;
		float CursorSpeed;
		bool bIsVisible;
		CursorConfig Config;
	};
}

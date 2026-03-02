// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"

namespace we
{
	class CursorSubsytem
	{
	public:
		explicit CursorSubsytem(const EngineConfig::CursorConfig& Config);

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

	private:
		void ApplyCursorSize();
		void CenterCursor();
		vec2f GetWindowSize() const;

	private:
		EngineConfig::CursorConfig Config;
		//texture CursorTexture;
		//optional<sprite> CursorSprite;
		vec2f CursorSize;
		float CursorSpeed;
		bool bIsVisible;
	};
}
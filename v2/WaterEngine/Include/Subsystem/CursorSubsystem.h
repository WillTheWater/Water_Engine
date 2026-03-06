// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	class CursorSubsystem
	{
	public:
		explicit CursorSubsystem();

		void SetTexture(shared<texture> Texture);

		void SetVisibility(bool Visible) { bIsVisible = Visible; }
		bool IsVisible() const { return bIsVisible; }

		void SetPosition(vec2f Position);
		vec2f GetPosition() const { return CurrentPosition; }

		void SetSize(vec2f NewSize) { CursorSize = NewSize; }
		vec2f GetSize() const { return CursorSize; }

		const drawable* GetDrawable() const;

	private:
		shared<texture> CursorTexture;
		optional<sprite> CursorSprite;

		vec2f CurrentPosition;
		vec2f CursorSize;
		bool bIsVisible;
	};
}
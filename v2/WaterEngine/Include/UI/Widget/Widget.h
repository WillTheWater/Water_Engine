// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"

namespace we
{
	class GameWindow;

	class Widget
	{
	public:
		virtual ~Widget() = default;

		void SetPosition(const vec2f& Pos) { Position = Pos; }
		vec2f GetPosition() const { return Position; }

		void SetSize(const vec2f& InSize) { Size = InSize; }
		vec2f GetSize() const { return Size; }

		void SetVisible(bool bInVisible) { bVisible = bInVisible; }
		bool IsVisible() const { return bVisible; }

		virtual void Update(float DeltaTime) {}
		virtual void Render(GameWindow& Window) {}
		virtual bool HandleClick(const vec2f& MousePos) { return false; }

		bool Contains(const vec2f& Point) const;

	protected:
		vec2f Position{ 0, 0 };
		vec2f Size{ 100, 50 };
		bool bVisible = true;
	};
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h
#include "UI/Widget/Widget.h"
#include "Utility/Delegate.h"

namespace we
{
	enum class ProgressDirection
	{
		LeftToRight,
		RightToLeft,
		BottomToTop,
		TopToBottom
	};

	class ProgressBar : public Widget
	{
	public:
		ProgressBar(EngineSubsystem& Subsystem,
			const vec2f& InSize = { 200.f, 24.f },
			float InitialValue = 0.f);

		float GetValue() const { return Value; }
		void SetValue(float NewValue);

		void SetDirection(ProgressDirection InDirection);
		ProgressDirection GetDirection() const { return Direction; }

		void SetBackgroundColor(color Color);
		void SetFillColor(color Color);

		void Update(float DeltaTime) override;
		void Render(GameWindow& Window) override;

	private:
		optional<rectangle> Background;
		optional<rectangle> Fill;

		float Value = 0.f;
		ProgressDirection Direction = ProgressDirection::LeftToRight;
	};
}

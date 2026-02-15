// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#pragma once

#include "Core/CoreMinimal.h
#include "UI/Widget/Widget.h"
#include "Utility/Delegate.h"

namespace we
{
	class Slider : public Widget
	{
	public:
		Slider(EngineSubsystem& Subsystem,
			const vec2f& InTrackSize = { 300.f, 20.f },
			const vec2f& InThumbSize = { 20.f, 30.f });

		// Value in range 0..1
		float GetValue() const { return Value; }
		void SetValue(float NewValue);

		// Event when slider moves
		Delegate<float> OnValueChanged;

		// Widget overrides
		void Update(float DeltaTime) override;
		void Render(GameWindow& Window) override;
		void OnMouseEnter() override;
		void OnMouseLeave() override;
		bool OnMouseButtonPressed() override;
		void OnMouseButtonReleased() override;
		void OnMouseMoved(const vec2f& MousePos) override;
		bool ShouldCaptureMouse() const override { return bGrabbed; }

	private:
		optional<rectangle> Track;
		optional<rectangle> Thumb;

		vec2f TrackSize;
		vec2f ThumbSize;

		float Value = 0.f;     // 0..1
		bool bGrabbed = false;
		bool bHovered = false;

		void UpdateThumbPosition();
		void UpdateVisualState();
	};
}

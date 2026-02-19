// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/Widget/Widget.h"
#include "Utility/Delegate.h"

namespace we
{
	enum class SliderOrientation
	{
		Horizontal,
		Vertical
	};

	class Slider : public Widget
	{
	public:
		// Rectangle slider (track and thumb are rectangles)
		Slider(
			float TrackWidth = 200.f,
			float TrackHeight = 10.f,
			float ThumbWidth = 20.f,
			float ThumbHeight = 30.f,
			SliderOrientation Orient = SliderOrientation::Horizontal);

		// Texture track + rectangle thumb
		Slider(
			const string& TrackTexturePath,
			float ThumbWidth,
			float ThumbHeight,
			SliderOrientation Orient = SliderOrientation::Horizontal);

		// Rectangle track + texture thumb
		Slider(
			float TrackWidth,
			float TrackHeight,
			const string& ThumbTexturePath,
			SliderOrientation Orient = SliderOrientation::Horizontal);

		// Texture track + texture thumb
		Slider(
			const string& TrackTexturePath,
			const string& ThumbTexturePath,
			SliderOrientation Orient = SliderOrientation::Horizontal);

		// Value in range 0..1
		float GetValue() const { return Value; }
		void SetValue(float NewValue);

		// Event when slider value changes
		Delegate<float> OnValueChanged;

		// Widget overrides
		void CollectRenderDepths(vector<RenderDepth>& OutDepths) const override;
		void SetPressed(bool bInPressed) override;
		void OnPressed(const vec2f& MousePos) override;
		bool Contains(const vec2f& ScreenPoint) const override;

		// Widget override - called when mouse moves while this widget is pressed (for dragging)
		void OnDrag(const vec2f& MousePos) override;

		// Visual customization
		void SetTrackColor(color InColor);
		void SetThumbColor(color InColor);
		void SetThumbHoverColor(color InColor);
		void SetThumbPressedColor(color InColor);

		// Texture setters (can be called after construction to switch to texture)
		void SetTrackTexture(const string& TexturePath);
		void SetThumbTexture(const string& TexturePath);

		// Set texture rects for thumb states (if using texture thumb)
		void SetThumbNormalRect(const recti& Rect);
		void SetThumbHoverRect(const recti& Rect);
		void SetThumbPressedRect(const recti& Rect);

	private:
		void InitializeShapes();
		void UpdateThumbPosition() const;
		void UpdateVisualState();
		float CalculateValueFromMouse(const vec2f& MousePos) const;

	private:
		// Geometry
		vec2f TrackSize;
		vec2f ThumbSize;
		SliderOrientation Orientation;

		// State
		float Value = 0.f;  // 0..1
		bool bGrabbed = false;
		bool bHovered = false;

		// Visuals - Track
		mutable optional<rectangle> TrackRect;
		mutable optional<sprite> TrackSprite;
		shared<texture> TrackTexture;
		color TrackColor{ 150, 150, 150 };
		bool bTrackUsesTexture = false;

		// Visuals - Thumb
		mutable optional<rectangle> ThumbRect;
		mutable optional<sprite> ThumbSprite;
		shared<texture> ThumbTexture;
		color ThumbNormalColor{ 100, 100, 100 };
		color ThumbHoverColor{ 150, 150, 150 };
		color ThumbPressedColor{ 80, 80, 80 };
		bool bThumbUsesTexture = false;

		// Thumb texture rects for states
		recti ThumbNormalRect;
		recti ThumbHoverRect;
		recti ThumbPressedRect;
		bool bThumbHasStateRects = false;
	};
}

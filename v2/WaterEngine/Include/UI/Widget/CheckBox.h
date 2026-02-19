// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/Widget/Widget.h"

namespace we
{
	enum class CheckBoxShape
	{
		Rectangle,
		Circle
	};

	class CheckBox : public Widget
	{
	public:
		// Shape-based checkbox (rectangle or circle)
		CheckBox(
			bool bInChecked = false,
			float BoxSize = 24.f,
			CheckBoxShape Shape = CheckBoxShape::Rectangle);

		// Texture-based checkbox
		CheckBox(
			const string& TexturePath,
			bool bInChecked = false,
			float BoxSize = 0.f);

		void CollectRenderDepths(vector<RenderDepth>& OutDepths) const override;

		// Focus point is the center of the checkbox
		vec2f GetFocusPoint() const override { return GetWorldPosition() + vec2f{ BoxSize * 0.5f, BoxSize * 0.5f }; }

		void SetChecked(bool bInChecked);
		bool IsChecked() const { return bChecked; }
		void Toggle();

		void SetNormalColor(color InColor);
		void SetCheckedColor(color InColor);
		void SetHoverColor(color InColor);

		void SetPressed(bool bInPressed) override;

		bool Contains(const vec2f& ScreenPoint) const override;

		Delegate<bool> OnToggled;

	private:
		void UpdateVisualState();
		void OnHoverGainedHandler();
		void OnHoverLostHandler();
		void OnClickHandler();

		bool bChecked = false;
		bool bHovered = false;
		bool bPressed = false;

		float BoxSize = 24.f;

		color NormalColor{ 80, 80, 80 };
		color CheckedColor{ 100, 200, 100 };
		color HoverColor{ 150, 150, 150 };

		CheckBoxShape ShapeType = CheckBoxShape::Rectangle;
		bool bUseTexture = false;

		mutable optional<rectangle> BoxRect;
		mutable optional<circle> BoxCircle;
		mutable optional<sprite> BoxSprite;
		shared<texture> BoxTexture;
	};
}

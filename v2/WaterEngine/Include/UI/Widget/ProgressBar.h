// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/Widget/Widget.h"

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
		// Rectangle progress bar (background and fill are rectangles)
		ProgressBar(
			const vec2f& Size = { 200.f, 24.f },
			float InitialValue = 0.f);

		// Texture background + rectangle fill
		ProgressBar(
			const string& BackgroundTexturePath,
			const vec2f& Size,
			float InitialValue = 0.f);

		// Rectangle background + texture fill
		ProgressBar(
			const vec2f& Size,
			const string& FillTexturePath,
			float InitialValue = 0.f);

		// Texture background + texture fill
		ProgressBar(
			const string& BackgroundTexturePath,
			const string& FillTexturePath,
			float InitialValue = 0.f);

		float GetValue() const { return Value; }
		void SetValue(float NewValue);

		void SetDirection(ProgressDirection InDirection);
		ProgressDirection GetDirection() const { return Direction; }

		// Color setters (for rectangle mode)
		void SetBackgroundColor(color Color);
		void SetFillColor(color Color);

		// Texture setters (can switch to texture after construction)
		void SetBackgroundTexture(const string& TexturePath);
		void SetFillTexture(const string& TexturePath);

		// Widget overrides
		void CollectRenderDepths(vector<RenderDepth>& OutDepths) const override;

	private:
		void InitializeShapes();
		void UpdateFillGeometry() const;

	private:
		vec2f BarSize;
		float Value = 0.f;
		ProgressDirection Direction = ProgressDirection::LeftToRight;

		// Background visual
		mutable optional<rectangle> BackgroundRect;
		mutable optional<sprite> BackgroundSprite;
		shared<texture> BackgroundTexture;
		color BackgroundColor{ 50, 50, 50 };
		bool bBackgroundUsesTexture = false;

		// Fill visual
		mutable optional<rectangle> FillRect;
		mutable optional<sprite> FillSprite;
		shared<texture> FillTexture;
		color FillColor{ 100, 200, 100 };
		bool bFillUsesTexture = false;
	};
}

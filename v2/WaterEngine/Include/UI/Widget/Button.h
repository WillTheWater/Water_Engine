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
	class Button : public Widget
	{
	public:
		// Rectangle-based button with text label
		Button(
			const string& InLabel,
			const vec2f& InSize,
			color FillColor = color{ 200, 200, 200 },
			color OutlineColor = color::Black,
			float OutlineThickness = 2.f);

		// Texture-based button with optional color tinting or texture rect states
		Button(
			const string& InLabel,
			const string& TexturePath,
			const vec2f& InSize = { 0.f, 0.f },
			bool bInUseColorTint = false);

		void CollectRenderDepths(vector<RenderDepth>& OutDepths) const override;

		void SetNormalColor(color InColor);
		void SetHoverColor(color InColor);
		void SetPressedColor(color InColor);

		void SetNormalTextureRect(const recti& Rect);
		void SetHoverTextureRect(const recti& Rect);
		void SetPressedTextureRect(const recti& Rect);

		void SetPressed(bool bInPressed) override;
		bool IsPressed() const { return bPressed; }
		bool IsHovered() const { return bHovered; }

	private:
		void UpdateVisualState();
		void OnButtonHoverGained();
		void OnButtonHoverLost();
		void LoadTexture(shared<texture>& OutTexture, const string& Path);

		string Label;
		mutable optional<text> LabelText;
		shared<font> TextFont;

		mutable rectangle BackgroundRect;

		shared<texture> ButtonTexture;
		mutable optional<sprite> BackgroundSprite;
		bool bUseTexture = false;
		bool bUseColorTint = false;

		recti NormalTextureRect;
		recti HoverTextureRect;
		recti PressedTextureRect;
		bool bHasTextureRects = false;

		bool bHovered = false;
		bool bPressed = false;

		color NormalColor{ 200, 200, 200 };
		color HoverColor{ 230, 230, 230 };
		color PressedColor{ 150, 150, 150 };
		color OutlineColor{ color::Black };
		float OutlineThickness = 2.f;
	};
}

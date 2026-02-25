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

		// Focus point is 90% across (left side) and centered vertically
		// This prevents cursor from covering button text
		vec2f GetFocusPoint() const override { 
			return GetWorldPosition() + vec2f(GetSize().x * 0.9f, GetSize().y * 0.5f); 
		}

		void SetNormalColor(color InColor);
		void SetHoverColor(color InColor);
		void SetPressedColor(color InColor);

		void SetNormalTextureRect(const recti& Rect);
		void SetHoverTextureRect(const recti& Rect);
		void SetPressedTextureRect(const recti& Rect);

		void SetPressed(bool bInPressed) override;
		bool IsPressed() const { return bPressed; }
		bool IsHovered() const { return bHovered; }

		// Label text
		void SetLabel(const string& InLabel);
		const string& GetLabel() const { return Label; }

		// Font management
		void SetFont(shared<font> InFont);
		void SetFont(const string& FontPath);
		shared<font> GetFont() const { return TextFont; }

		// Text size (character size in pixels)
		void SetTextSize(uint InSize);
		uint GetTextSize() const { return TextSize; }

	private:
		void UpdateVisualState();
		void OnButtonHoverGained();
		void OnButtonHoverLost();
		void LoadTexture(shared<texture>& OutTexture, const string& Path);
		void RebuildText();

	private:
		string Label;
		mutable optional<text> LabelText;
		shared<font> TextFont;
		uint TextSize = 24;  // Default character size

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

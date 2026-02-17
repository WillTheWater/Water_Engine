// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"
#include "UI/Widget/Widget.h"

namespace we
{
	enum class TextAlignment
	{
		Left,
		Center,
		Right
	};

	class TextBlock : public Widget
	{
	public:
		// TextBlock with auto-wrap at specified width (0 = no wrap, auto-size)
		TextBlock(
			const string& InText = "",
			float InWrapWidth = 0.f,  // 0 = no wrapping, use full text width
			uint InCharSize = 24);

		// Set text content
		void SetText(const string& InText);
		const string& GetText() const { return RawText; }

		// Font management
		void SetFont(const string& FontPath);
		void SetFont(shared<font> InFont);

		// Appearance
		void SetCharacterSize(uint Size);
		void SetColor(const color& InColor);
		void SetLineSpacing(float Spacing);
		void SetOutlineColor(const color& InColor);
		void SetOutlineThickness(float Thickness);

		// Alignment
		void SetAlignment(TextAlignment Align);
		TextAlignment GetAlignment() const { return Alignment; }

		// Wrap width (0 = no wrap)
		void SetWrapWidth(float Width);

		// Widget overrides
		void CollectRenderDepths(vector<RenderDepth>& OutDepths) const override;

	private:
		void RebuildLines();
		float CalculateTextWidth(const string& Text) const;

	private:
		string RawText;
		shared<font> FontResource;
		mutable vector<optional<text>> LineDrawables;
		vector<string> Lines;
		mutable vec2f TextSize;

		uint CharSize = 24;
		float LineSpacing = 1.2f;
		float WrapWidth = 0.f;
		TextAlignment Alignment = TextAlignment::Left;
		color TextColor = color::White;
		color OutlineColor = color::Transparent;
		float OutlineThickness = 0.f;
		bool bNeedsRebuild = true;
	};
}

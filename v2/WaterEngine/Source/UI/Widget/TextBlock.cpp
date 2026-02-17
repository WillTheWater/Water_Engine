// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/TextBlock.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"
#include "Utility/Log.h"

namespace we
{
	TextBlock::TextBlock(
		const string& InText,
		float InWrapWidth,
		uint InCharSize)
		: RawText(InText)
		, CharSize(InCharSize)
		, WrapWidth(InWrapWidth)
	{
		// Load default font
		FontResource = LoadAsset().LoadFontSync(EC.DefaultFont);
		TextColor = color::White;
		bNeedsRebuild = true;
	}

	void TextBlock::SetText(const string& InText)
	{
		if (RawText != InText)
		{
			RawText = InText;
			bNeedsRebuild = true;
		}
	}

	void TextBlock::SetFont(const string& FontPath)
	{
		if (!FontPath.empty())
		{
			FontResource = LoadAsset().LoadFontSync(FontPath);
			bNeedsRebuild = true;
		}
	}

	void TextBlock::SetFont(shared<font> InFont)
	{
		if (FontResource != InFont)
		{
			FontResource = InFont;
			bNeedsRebuild = true;
		}
	}

	void TextBlock::SetCharacterSize(uint Size)
	{
		if (CharSize != Size)
		{
			CharSize = Size;
			bNeedsRebuild = true;
		}
	}

	void TextBlock::SetColor(const color& InColor)
	{
		TextColor = InColor;
		// Update existing line drawables
		for (auto& Line : LineDrawables)
		{
			if (Line)
			{
				Line->setFillColor(TextColor);
			}
		}
	}

	void TextBlock::SetOutlineColor(const color& InColor)
	{
		OutlineColor = InColor;
		// Update existing line drawables
		for (auto& Line : LineDrawables)
		{
			if (Line)
			{
				Line->setOutlineColor(OutlineColor);
			}
		}
	}

	void TextBlock::SetOutlineThickness(float Thickness)
	{
		OutlineThickness = Thickness;
		// Update existing line drawables
		for (auto& Line : LineDrawables)
		{
			if (Line)
			{
				Line->setOutlineThickness(OutlineThickness);
			}
		}
	}

	void TextBlock::SetLineSpacing(float Spacing)
	{
		if (LineSpacing != Spacing)
		{
			LineSpacing = Spacing;
			bNeedsRebuild = true;
		}
	}

	void TextBlock::SetAlignment(TextAlignment Align)
	{
		if (Alignment != Align)
		{
			Alignment = Align;
			bNeedsRebuild = true;
		}
	}

	void TextBlock::SetWrapWidth(float Width)
	{
		if (WrapWidth != Width)
		{
			WrapWidth = Width;
			bNeedsRebuild = true;
		}
	}

	float TextBlock::CalculateTextWidth(const string& Text) const
	{
		if (!FontResource) return 0.f;

		float Width = 0.f;
		for (char c : Text)
		{
			const auto& Glyph = FontResource->getGlyph(static_cast<std::uint32_t>(c), CharSize, false);
			Width += Glyph.advance;
		}
		return Width;
	}

	void TextBlock::RebuildLines()
	{
		if (!FontResource) return;

		Lines.clear();
		LineDrawables.clear();

		if (RawText.empty())
		{
			TextSize = { 0.f, 0.f };
			SetSize(TextSize);
			bNeedsRebuild = false;
			return;
		}

		float LineHeight = FontResource->getLineSpacing(CharSize) * LineSpacing;
		float MaxLineWidth = 0.f;
		int LineCount = 0;

		// Word wrapping logic
		if (WrapWidth > 0.f)
		{
			// Word wrapping enabled
			string CurrentLine;
			string CurrentWord;
			float CurrentLineWidth = 0.f;
			float CurrentWordWidth = 0.f;
			float SpaceWidth = FontResource->getGlyph(' ', CharSize, false).advance;

			auto FlushLine = [&]()
			{
				if (!CurrentLine.empty() || !CurrentWord.empty())
				{
					CurrentLine += CurrentWord;
					Lines.push_back(CurrentLine);
					MaxLineWidth = std::max(MaxLineWidth, CurrentLineWidth + CurrentWordWidth);
					CurrentLine.clear();
					CurrentWord.clear();
					CurrentLineWidth = 0.f;
					CurrentWordWidth = 0.f;
					LineCount++;
				}
			};

			for (size_t i = 0; i < RawText.length(); ++i)
			{
				char c = RawText[i];

				if (c == '\n')
				{
					FlushLine();
				}
				else if (c == ' ' || c == '\t')
				{
					// Check if adding this word would exceed wrap width
					if (CurrentLineWidth + CurrentWordWidth > WrapWidth && !CurrentLine.empty())
					{
						// Start new line with current word
						Lines.push_back(CurrentLine);
						MaxLineWidth = std::max(MaxLineWidth, CurrentLineWidth);
						CurrentLine = CurrentWord + " ";
						CurrentLineWidth = CurrentWordWidth + SpaceWidth;
						CurrentWord.clear();
						CurrentWordWidth = 0.f;
						LineCount++;
					}
					else
					{
						CurrentLine += CurrentWord + " ";
						CurrentLineWidth += CurrentWordWidth + SpaceWidth;
						CurrentWord.clear();
						CurrentWordWidth = 0.f;
					}
				}
				else
				{
					CurrentWord += c;
					const auto& Glyph = FontResource->getGlyph(static_cast<std::uint32_t>(c), CharSize, false);
					CurrentWordWidth += Glyph.advance;
				}
			}

			// Flush remaining content
			FlushLine();
		}
		else
		{
			// No wrapping - split by newlines only
			string CurrentLine;
			for (char c : RawText)
			{
				if (c == '\n')
				{
					if (!CurrentLine.empty())
					{
						Lines.push_back(CurrentLine);
						MaxLineWidth = std::max(MaxLineWidth, CalculateTextWidth(CurrentLine));
						LineCount++;
					}
					CurrentLine.clear();
				}
				else
				{
					CurrentLine += c;
				}
			}
			if (!CurrentLine.empty())
			{
				Lines.push_back(CurrentLine);
				MaxLineWidth = std::max(MaxLineWidth, CalculateTextWidth(CurrentLine));
				LineCount++;
			}
		}

		// If no lines were created (empty text after processing), add empty line
		if (Lines.empty())
		{
			Lines.push_back("");
			LineCount = 1;
		}

		// Create text drawables for each line
		LineDrawables.reserve(Lines.size());
		for (const auto& Line : Lines)
		{
			if (FontResource)
			{
				LineDrawables.emplace_back();
				LineDrawables.back().emplace(*FontResource, Line, CharSize);
				LineDrawables.back()->setFillColor(TextColor);
				LineDrawables.back()->setOutlineColor(OutlineColor);
				LineDrawables.back()->setOutlineThickness(OutlineThickness);
			}
			else
			{
				LineDrawables.emplace_back();
			}
		}

		// Calculate total size
		float TotalWidth = WrapWidth > 0.f ? WrapWidth : MaxLineWidth;
		float TotalHeight = LineHeight * static_cast<float>(LineCount);
		TextSize = { TotalWidth, TotalHeight };
		SetSize(TextSize);

		bNeedsRebuild = false;
	}

	void TextBlock::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		if (!IsVisible()) return;

		// Rebuild lines if needed - need to cast away const
		if (bNeedsRebuild)
		{
			const_cast<TextBlock*>(this)->RebuildLines();
		}

		if (Lines.empty()) return;

		vec2f Pos = GetWorldPosition();
		float Depth = GetRenderDepth();
		float LineHeight = FontResource ? FontResource->getLineSpacing(CharSize) * LineSpacing : CharSize;

		// Calculate the actual max line width for alignment
		float MaxLineWidth = 0.f;
		if (Alignment != TextAlignment::Left)
		{
			for (const auto& Line : Lines)
			{
				MaxLineWidth = std::max(MaxLineWidth, CalculateTextWidth(Line));
			}
		}

		float CurrentY = Pos.y;
		for (size_t i = 0; i < Lines.size() && i < LineDrawables.size(); ++i)
		{
			if (!LineDrawables[i]) continue;

			float LineX = Pos.x;

			// Apply alignment
			if (Alignment == TextAlignment::Center)
			{
				float LineWidth = CalculateTextWidth(Lines[i]);
				LineX = Pos.x + (TextSize.x - LineWidth) * 0.5f;
			}
			else if (Alignment == TextAlignment::Right)
			{
				float LineWidth = CalculateTextWidth(Lines[i]);
				LineX = Pos.x + TextSize.x - LineWidth;
			}

			LineDrawables[i]->setPosition({ LineX, CurrentY });
			OutDepths.push_back({ &*LineDrawables[i], Depth });

			CurrentY += LineHeight;
		}
	}
}

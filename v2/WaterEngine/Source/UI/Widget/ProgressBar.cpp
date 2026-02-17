// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/ProgressBar.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	// =========================================================================
	// Constructors
	// =========================================================================

	// Rectangle progress bar
	ProgressBar::ProgressBar(
		const vec2f& InSize,
		float InitialValue)
		: BarSize(InSize)
		, Value(std::clamp(InitialValue, 0.f, 1.f))
		, bBackgroundUsesTexture(false)
		, bFillUsesTexture(false)
	{
		InitializeShapes();
	}

	// Texture background + rectangle fill
	ProgressBar::ProgressBar(
		const string& BackgroundTexturePath,
		const vec2f& InSize,
		float InitialValue)
		: BarSize(InSize)
		, Value(std::clamp(InitialValue, 0.f, 1.f))
		, bFillUsesTexture(false)
	{
		SetBackgroundTexture(BackgroundTexturePath);
		// If texture failed to load, use default size
		if (!bBackgroundUsesTexture)
		{
			BarSize = InSize;
		}
		InitializeShapes();
	}

	// Rectangle background + texture fill
	ProgressBar::ProgressBar(
		const vec2f& InSize,
		const string& FillTexturePath,
		float InitialValue)
		: BarSize(InSize)
		, Value(std::clamp(InitialValue, 0.f, 1.f))
		, bBackgroundUsesTexture(false)
	{
		SetFillTexture(FillTexturePath);
		// If fill texture failed, keep the specified size
		InitializeShapes();
	}

	// Texture background + texture fill
	ProgressBar::ProgressBar(
		const string& BackgroundTexturePath,
		const string& FillTexturePath,
		float InitialValue)
		: Value(std::clamp(InitialValue, 0.f, 1.f))
	{
		SetBackgroundTexture(BackgroundTexturePath);
		SetFillTexture(FillTexturePath);
		// If background texture failed, use a default size
		if (!bBackgroundUsesTexture)
		{
			BarSize = { 200.f, 24.f };
		}
		InitializeShapes();
	}

	void ProgressBar::InitializeShapes()
	{
		// Background setup
		if (!bBackgroundUsesTexture)
		{
			BackgroundRect.emplace();
			BackgroundRect->setSize(BarSize);
			BackgroundRect->setFillColor(BackgroundColor);
		}

		// Fill setup
		if (!bFillUsesTexture)
		{
			FillRect.emplace();
			FillRect->setFillColor(FillColor);
		}

		SetSize(BarSize);
	}

	// =========================================================================
	// Value Management
	// =========================================================================

	void ProgressBar::SetValue(float NewValue)
	{
		Value = std::clamp(NewValue, 0.f, 1.f);
	}

	void ProgressBar::SetDirection(ProgressDirection InDirection)
	{
		Direction = InDirection;
	}

	// =========================================================================
	// Rendering
	// =========================================================================

	void ProgressBar::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		if (!IsVisible()) return;

		vec2f Pos = GetWorldPosition();
		float Depth = GetRenderDepth();

		// Background
		if (bBackgroundUsesTexture && BackgroundSprite)
		{
			BackgroundSprite->setPosition(Pos);
			OutDepths.push_back({ &*BackgroundSprite, Depth });
		}
		else if (BackgroundRect)
		{
			BackgroundRect->setPosition(Pos);
			OutDepths.push_back({ &*BackgroundRect, Depth });
		}

		// Fill (at higher depth)
		if (Value > 0.f)
		{
			UpdateFillGeometry();
			if (bFillUsesTexture && FillSprite)
			{
				OutDepths.push_back({ &*FillSprite, Depth + 1.f });
			}
			else if (FillRect)
			{
				OutDepths.push_back({ &*FillRect, Depth + 1.f });
			}
		}
	}

	void ProgressBar::UpdateFillGeometry() const
	{
		vec2f Pos = GetWorldPosition();
		vec2f FillPos = Pos;
		vec2f FillSize = BarSize;

		switch (Direction)
		{
		case ProgressDirection::LeftToRight:
			FillSize = { BarSize.x * Value, BarSize.y };
			break;

		case ProgressDirection::RightToLeft:
			FillSize = { BarSize.x * Value, BarSize.y };
			FillPos = Pos + vec2f{ BarSize.x - FillSize.x, 0.f };
			break;

		case ProgressDirection::TopToBottom:
			FillSize = { BarSize.x, BarSize.y * Value };
			break;

		case ProgressDirection::BottomToTop:
			FillSize = { BarSize.x, BarSize.y * Value };
			FillPos = Pos + vec2f{ 0.f, BarSize.y - FillSize.y };
			break;
		}

		if (FillRect)
		{
			FillRect->setPosition(FillPos);
			FillRect->setSize(FillSize);
		}
		else if (FillSprite)
		{
			FillSprite->setPosition(FillPos);
			// Scale the sprite to match the fill size
			vec2f TextureSize = vec2f(FillTexture->getSize());
			vec2f Scale = { FillSize.x / TextureSize.x, FillSize.y / TextureSize.y };
			FillSprite->setScale(Scale);
		}
	}

	// =========================================================================
	// Setters
	// =========================================================================

	void ProgressBar::SetBackgroundColor(color Color)
	{
		BackgroundColor = Color;
		if (BackgroundRect)
		{
			BackgroundRect->setFillColor(BackgroundColor);
		}
	}

	void ProgressBar::SetFillColor(color Color)
	{
		FillColor = Color;
		if (FillRect)
		{
			FillRect->setFillColor(FillColor);
		}
	}

	void ProgressBar::SetBackgroundTexture(const string& TexturePath)
	{
		if (!TexturePath.empty())
		{
			BackgroundTexture = LoadAsset().LoadTextureSync(TexturePath);
			if (BackgroundTexture)
			{
				BarSize = vec2f(BackgroundTexture->getSize());
				bBackgroundUsesTexture = true;
				BackgroundRect.reset();
				BackgroundSprite.emplace(*BackgroundTexture);
				SetSize(BarSize);
			}
		}
	}

	void ProgressBar::SetFillTexture(const string& TexturePath)
	{
		if (!TexturePath.empty())
		{
			FillTexture = LoadAsset().LoadTextureSync(TexturePath);
			if (FillTexture)
			{
				bFillUsesTexture = true;
				FillRect.reset();
				FillSprite.emplace(*FillTexture);
			}
		}
	}
}

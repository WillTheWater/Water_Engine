// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Panel.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	Panel::Panel(
		const vec2f& InSize,
		color InFillColor,
		color InOutlineColor,
		float InOutlineThickness)
		: FillColor(InFillColor)
		, OutlineColor(InOutlineColor)
		, OutlineThickness(InOutlineThickness)
		, bUseTexture(false)
	{
		SetSize(InSize);

		BackgroundRect.setSize(InSize);
		BackgroundRect.setFillColor(FillColor);
		BackgroundRect.setOutlineColor(OutlineColor);
		BackgroundRect.setOutlineThickness(OutlineThickness);
	}

	Panel::Panel(
		const string& TexturePath,
		const vec2f& InSize)
		: bUseTexture(true)
	{
		if (!TexturePath.empty())
		{
			BackgroundTexture = LoadAsset().LoadTextureSync(TexturePath);
		}

		if (BackgroundTexture)
		{
			vec2f TexSize = vec2f(BackgroundTexture->getSize());
			
			if (InSize.x == 0.f || InSize.y == 0.f)
			{
				SetSize(TexSize);
			}
			else
			{
				SetSize(InSize);
			}

			BackgroundSprite.emplace(*BackgroundTexture);
		}
		else
		{
			SetSize({ 300.f, 200.f });
			
			BackgroundRect.setSize(GetSize());
			BackgroundRect.setFillColor(FillColor);
			BackgroundRect.setOutlineColor(OutlineColor);
			BackgroundRect.setOutlineThickness(OutlineThickness);
			bUseTexture = false;
		}
	}

	void Panel::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		if (!IsVisible()) return;

		vec2f Pos = GetWorldPosition();
		vec2f Scl = GetWorldScale();

		if (bUseTexture && BackgroundSprite)
		{
			BackgroundSprite->setPosition(Pos);
			BackgroundSprite->setScale(Scl);
			OutDepths.push_back({ &*BackgroundSprite, GetRenderDepth() });
		}
		else
		{
			BackgroundRect.setPosition(Pos);
			BackgroundRect.setScale(Scl);
			OutDepths.push_back({ &BackgroundRect, GetRenderDepth() });
		}
	}

	void Panel::SetFillColor(color InColor)
	{
		FillColor = InColor;
		if (!bUseTexture)
		{
			BackgroundRect.setFillColor(FillColor);
		}
	}

	void Panel::SetOutlineColor(color InColor)
	{
		OutlineColor = InColor;
		if (!bUseTexture)
		{
			BackgroundRect.setOutlineColor(OutlineColor);
		}
	}

	void Panel::SetOutlineThickness(float Thickness)
	{
		OutlineThickness = Thickness;
		if (!bUseTexture)
		{
			BackgroundRect.setOutlineThickness(OutlineThickness);
		}
	}
}

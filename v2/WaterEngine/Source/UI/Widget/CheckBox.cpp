// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/CheckBox.h"
#include "UI/UIUtility.h"
#include "EngineConfig.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	CheckBox::CheckBox(
		bool bInChecked,
		float InBoxSize,
		CheckBoxShape Shape)
		: bChecked(bInChecked)
		, BoxSize(InBoxSize)
		, ShapeType(Shape)
		, bUseTexture(false)
	{
		SetFocusable(true);
		SetSize({ BoxSize, BoxSize });

		if (ShapeType == CheckBoxShape::Rectangle)
		{
			BoxRect.emplace();
			BoxRect->setSize({ BoxSize, BoxSize });
			BoxRect->setOutlineColor(color::Black);
			BoxRect->setOutlineThickness(2.f);
			BoxRect->setOrigin({ BoxSize * 0.5f, BoxSize * 0.5f });
		}
		else
		{
			BoxCircle.emplace();
			BoxCircle->setRadius(BoxSize * 0.5f);
			BoxCircle->setOutlineColor(color::Black);
			BoxCircle->setOutlineThickness(2.f);
			BoxCircle->setOrigin({ BoxSize * 0.5f, BoxSize * 0.5f });
		}

		UpdateVisualState();

		OnFocusGained.Bind(this, &CheckBox::OnFocusGainedHandler);
		OnFocusLost.Bind(this, &CheckBox::OnFocusLostHandler);
		OnClicked.Bind(this, &CheckBox::OnClickHandler);
	}

	CheckBox::CheckBox(
		const string& TexturePath,
		bool bInChecked,
		float InBoxSize)
		: bChecked(bInChecked)
		, bUseTexture(true)
	{
		SetFocusable(true);

		if (!TexturePath.empty())
		{
			BoxTexture = LoadAsset().LoadTextureSync(TexturePath);
		}

		if (BoxTexture)
		{
			BoxSize = (InBoxSize > 0.f) ? InBoxSize : vec2f(BoxTexture->getSize()).x;
			BoxSprite.emplace(*BoxTexture);
			BoxSprite->setOrigin({ BoxSize * 0.5f, BoxSize * 0.5f });
			SetSize({ BoxSize, BoxSize });
		}
		else
		{
			BoxSize = 24.f;
			BoxRect.emplace();
			BoxRect->setSize({ BoxSize, BoxSize });
			BoxRect->setOrigin({ BoxSize * 0.5f, BoxSize * 0.5f });
			bUseTexture = false;
			SetSize({ BoxSize, BoxSize });
		}

		OnFocusGained.Bind(this, &CheckBox::OnFocusGainedHandler);
		OnFocusLost.Bind(this, &CheckBox::OnFocusLostHandler);
		OnClicked.Bind(this, &CheckBox::OnClickHandler);
	}

	void CheckBox::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		if (!IsVisible()) return;

		vec2f Pos = GetWorldPosition();
		float Depth = GetRenderDepth();

		// All shapes have origin at center, so position directly
		if (bUseTexture && BoxSprite)
		{
			BoxSprite->setPosition(Pos + vec2f{ BoxSize * 0.5f, BoxSize * 0.5f });
			OutDepths.push_back({ &*BoxSprite, Depth });
		}
		else if (ShapeType == CheckBoxShape::Rectangle && BoxRect)
		{
			BoxRect->setPosition(Pos + vec2f{ BoxSize * 0.5f, BoxSize * 0.5f });
			OutDepths.push_back({ &*BoxRect, Depth });
		}
		else if (BoxCircle)
		{
			BoxCircle->setPosition(Pos + vec2f{ BoxSize * 0.5f, BoxSize * 0.5f });
			OutDepths.push_back({ &*BoxCircle, Depth });
		}
	}

	bool CheckBox::Contains(const vec2f& ScreenPoint) const
	{
		// Origin is at center for all checkbox types
		vec2f Pos = GetWorldPosition();
		vec2f Center = Pos + vec2f{ BoxSize * 0.5f, BoxSize * 0.5f };

		if (ShapeType == CheckBoxShape::Circle)
		{
			// Circle hit test: exact radius check from center
			float Radius = BoxSize * 0.5f;
			vec2f Delta = ScreenPoint - Center;
			float DistSq = Delta.lengthSquared();
			// Use <= for inside or on edge
			return DistSq <= (Radius * Radius);
		}
		else
		{
			// Rectangle hit test (with origin at center, so expand by half size)
			vec2f HalfSize = { BoxSize * 0.5f, BoxSize * 0.5f };
			return ScreenPoint.x >= Center.x - HalfSize.x &&
			       ScreenPoint.x <= Center.x + HalfSize.x &&
			       ScreenPoint.y >= Center.y - HalfSize.y &&
			       ScreenPoint.y <= Center.y + HalfSize.y;
		}
	}

	void CheckBox::SetChecked(bool bInChecked)
	{
		if (bChecked != bInChecked)
		{
			bChecked = bInChecked;
			UpdateVisualState();
			OnToggled.Broadcast(bChecked);
		}
	}

	void CheckBox::Toggle()
	{
		SetChecked(!bChecked);
	}

	void CheckBox::SetNormalColor(color InColor)
	{
		NormalColor = InColor;
		UpdateVisualState();
	}

	void CheckBox::SetCheckedColor(color InColor)
	{
		CheckedColor = InColor;
		UpdateVisualState();
	}

	void CheckBox::SetHoverColor(color InColor)
	{
		HoverColor = InColor;
		UpdateVisualState();
	}

	void CheckBox::SetPressed(bool bInPressed)
	{
		bPressed = bInPressed;
		UpdateVisualState();
	}

	void CheckBox::UpdateVisualState()
	{
		if (bUseTexture)
		{
			return;
		}

		color TargetColor;

		if (bPressed)
			TargetColor = bChecked ? CheckedColor : NormalColor;
		else if (bHovered)
			TargetColor = HoverColor;
		else if (bChecked)
			TargetColor = CheckedColor;
		else
			TargetColor = NormalColor;

		if (BoxRect)
		{
			BoxRect->setFillColor(TargetColor);
		}
		else if (BoxCircle)
		{
			BoxCircle->setFillColor(TargetColor);
		}
	}

	void CheckBox::OnFocusGainedHandler()
	{
		bHovered = true;
		UpdateVisualState();
	}

	void CheckBox::OnFocusLostHandler()
	{
		bHovered = false;
		bPressed = false;
		UpdateVisualState();
	}

	void CheckBox::OnClickHandler()
	{
		Toggle();
	}
}

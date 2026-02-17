// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/AutoPanel.h"

namespace we
{
	AutoPanel::AutoPanel(
		color InFillColor,
		color InOutlineColor,
		float InOutlineThickness,
		float InSpacing)
		: FillColor(InFillColor)
		, OutlineColor(InOutlineColor)
		, OutlineThickness(InOutlineThickness)
		, Spacing(InSpacing)
	{
		BackgroundRect.setFillColor(FillColor);
		BackgroundRect.setOutlineColor(OutlineColor);
		BackgroundRect.setOutlineThickness(OutlineThickness);
	}

	void AutoPanel::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		if (!IsVisible()) return;

		BackgroundRect.setPosition(GetWorldPosition());
		BackgroundRect.setScale(GetWorldScale());
		OutDepths.push_back({ &BackgroundRect, GetRenderDepth() });

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				Child->CollectRenderDepths(OutDepths);
			}
		}
	}

	void AutoPanel::AddChild(shared<Widget> Child, ChildAlignment Align, vec2f Offset)
	{
		Widget::AddChild(Child, Anchor::TopLeft, Anchor::TopLeft, Offset);
		ChildAlignments.push_back(Align);
		ChildOffsets.push_back(Offset);
		RefreshLayout();
	}

	void AutoPanel::SetFillColor(color InColor)
	{
		FillColor = InColor;
		BackgroundRect.setFillColor(FillColor);
	}

	void AutoPanel::SetOutlineColor(color InColor)
	{
		OutlineColor = InColor;
		BackgroundRect.setOutlineColor(OutlineColor);
	}

	void AutoPanel::SetOutlineThickness(float Thickness)
	{
		OutlineThickness = Thickness;
		BackgroundRect.setOutlineThickness(OutlineThickness);
	}

	void AutoPanel::SetBorder(float Left, float Right, float Top, float Bottom)
	{
		Border.position = { Left, Top };
		Border.size = { Right, Bottom };
		RefreshLayout();
	}

	void AutoPanel::SetBorder(const rectf& InBorder)
	{
		Border = InBorder;
		RefreshLayout();
	}

	void AutoPanel::RefreshLayout()
	{
		float ContentWidth = 0.f;
		float ContentHeight = 0.f;

		for (size_t i = 0; i < Children.size(); ++i)
		{
			if (auto Child = Children[i].lock())
			{
				vec2f ChildSize = Child->GetSize();
				ContentWidth = std::max(ContentWidth, ChildSize.x);
				ContentHeight += ChildSize.y;
				if (i > 0) ContentHeight += Spacing;
			}
		}

		float PanelWidth = Border.position.x + ContentWidth + Border.size.x;
		float PanelHeight = Border.position.y + ContentHeight + Border.size.y;

		SetSize({ PanelWidth, PanelHeight });
		BackgroundRect.setSize(GetSize());

		float CurrentY = Border.position.y;

		for (size_t i = 0; i < Children.size(); ++i)
		{
			if (auto Child = Children[i].lock())
			{
				vec2f ChildSize = Child->GetSize();
				float XOffset = Border.position.x;

				switch (ChildAlignments[i])
				{
				case ChildAlignment::Center:
					XOffset += (ContentWidth - ChildSize.x) * 0.5f;
					break;
				case ChildAlignment::Right:
					XOffset += ContentWidth - ChildSize.x;
					break;
				case ChildAlignment::Left:
				default:
					break;
				}

				XOffset += ChildOffsets[i].x;
				float YPos = CurrentY + ChildOffsets[i].y;

				Child->SetAnchorPosition(Anchor::TopLeft, Anchor::TopLeft, { XOffset, YPos });

				CurrentY += ChildSize.y + Spacing;
			}
		}
	}
}

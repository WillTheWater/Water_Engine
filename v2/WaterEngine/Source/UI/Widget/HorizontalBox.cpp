// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/HorizontalBox.h"

namespace we
{
	HorizontalBox::HorizontalBox(float InPadding)
		: Padding(InPadding)
	{
	}

	void HorizontalBox::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				Child->CollectRenderDepths(OutDepths);
			}
		}
	}

	void HorizontalBox::AddChild(shared<Widget> Child, Anchor InTargetAnchor, Anchor InWidgetAnchor, vec2f InOffset)
	{
		Widget::AddChild(Child, InTargetAnchor, InWidgetAnchor, InOffset);
		RefreshLayout();
	}

	void HorizontalBox::RefreshLayout()
	{
		float CurrentX = 0.f;
		float MaxHeight = 0.f;

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				vec2f ChildSize = Child->GetSize();
				MaxHeight = std::max(MaxHeight, ChildSize.y);
			}
		}

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				vec2f ChildSize = Child->GetSize();
				float YOffset = (MaxHeight - ChildSize.y) * 0.5f;

				Child->SetAnchorPosition(Anchor::TopLeft, Anchor::TopLeft, { CurrentX, YOffset });
				CurrentX += ChildSize.x + Padding;
			}
		}

		SetSize({ CurrentX > 0.f ? CurrentX - Padding : 0.f, MaxHeight });
	}
}

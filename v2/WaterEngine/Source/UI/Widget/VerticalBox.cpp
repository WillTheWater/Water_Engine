// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/VerticalBox.h"

namespace we
{
	VerticalBox::VerticalBox(float InPadding)
		: Padding(InPadding)
	{
	}

	void VerticalBox::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				Child->CollectRenderDepths(OutDepths);
			}
		}
	}

	void VerticalBox::AddChild(shared<Widget> Child, Anchor InTargetAnchor, Anchor InWidgetAnchor, vec2f InOffset)
	{
		Widget::AddChild(Child, InTargetAnchor, InWidgetAnchor, InOffset);
		RefreshLayout();
	}

	void VerticalBox::RefreshLayout()
	{
		float CurrentY = 0.f;
		float MaxWidth = 0.f;

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				vec2f ChildSize = Child->GetSize();
				MaxWidth = std::max(MaxWidth, ChildSize.x);
			}
		}

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				vec2f ChildSize = Child->GetSize();
				float XOffset = (MaxWidth - ChildSize.x) * 0.5f;

				Child->SetAnchorPosition(Anchor::TopLeft, Anchor::TopLeft, { XOffset, CurrentY });
				CurrentY += ChildSize.y + Padding;
			}
		}

		SetSize({ MaxWidth, CurrentY > 0.f ? CurrentY - Padding : 0.f });
	}
}

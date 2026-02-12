// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/VerticalBox.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/GameWindow.h"

namespace we
{
	VerticalBox::VerticalBox(
		EngineSubsystem& Subsystem,
		const list<shared<Widget>>& InChildren,
		float Padding,
		Anchor InTargetAnchor,
		Anchor InWidgetAnchor,
		const vec2f& InOffset
	)
		: Widget{ Subsystem }
	{
		float MinX = FLT_MAX, MinY = FLT_MAX;
		float MaxX = -FLT_MAX, MaxY = -FLT_MAX;

		float CursorY = 0.f;

		// ------------------------------------------------------------
		// Attach children and compute stacked bounds (local space)
		// ------------------------------------------------------------

		for (const auto& Child : InChildren)
		{
			if (!Child)
				continue;

			Child->SetParent(this);
			Children.push_back(Child);

			// IMPORTANT:
			// We do NOT change anchors here.
			// We only simulate vertical stacking by offsetting Y.
			const vec2f ChildPos =
				Child->AnchorOffset +
				Child->LocalOffset +
				vec2f{ 0.f, CursorY };

			const vec2f ChildScale = Child->GetWorldScale();
			const vec2f ChildSize = Child->GetSize().componentWiseMul(ChildScale);
			const vec2f ChildOrigin = Child->GetOrigin().componentWiseMul(ChildScale);

			const float Left = ChildPos.x - ChildOrigin.x;
			const float Top = ChildPos.y - ChildOrigin.y;
			const float Right = Left + ChildSize.x;
			const float Bottom = Top + ChildSize.y;

			MinX = std::min(MinX, Left);
			MinY = std::min(MinY, Top);
			MaxX = std::max(MaxX, Right);
			MaxY = std::max(MaxY, Bottom);

			CursorY += ChildSize.y + Padding;
		}

		// ------------------------------------------------------------
		// VerticalBox size = stacked content size
		// ------------------------------------------------------------

		const float ContentWidth = MaxX - MinX;
		const float ContentHeight = MaxY - MinY;

		const vec2f BoxSize = { ContentWidth, ContentHeight };
		SetSize(BoxSize);

		// ------------------------------------------------------------
		// Offset children so stacked content starts at (0,0)
		// EXACTLY like Panel
		// ------------------------------------------------------------

		float ApplyY = 0.f;

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				const vec2f ChildScale =
					Child->GetSize().componentWiseMul(Child->GetWorldScale());

				const vec2f NewAnchorOffset =
					Child->AnchorOffset +
					vec2f{ -MinX, ApplyY - MinY };

				Child->SetAnchorPosition(
					Anchor::TopLeft,
					Child->WidgetAnchor,
					NewAnchorOffset
				);

				ApplyY += ChildScale.y + Padding;
			}
		}

		// ------------------------------------------------------------
		// Anchor the VerticalBox itself
		// ------------------------------------------------------------

		SetAnchorPosition(
			InTargetAnchor,
			InWidgetAnchor,
			InOffset
		);
	}
}
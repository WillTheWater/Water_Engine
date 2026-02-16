// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Button.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Utility/Log.h"
#include "UI/Widget/HorizontalBox.h"

namespace we
{
	HorizontalBox::HorizontalBox(
		EngineSubsystem& Subsystem,
		const vector<shared<Widget>>& InChildren,
		float Padding,
		Anchor InTargetAnchor,
		Anchor InWidgetAnchor,
		const vec2f& InOffset
	)
		: Widget{ Subsystem }
	{
		float MinX = FLT_MAX;
		float MaxX = -FLT_MAX;
		float MaxY = -FLT_MAX;

		float CursorX = 0.f;

		// ------------------------------------------------------------
		// Attach children and compute stacked bounds (local space)
		// ------------------------------------------------------------

		for (const auto& Child : InChildren)
		{
			if (!Child)
				continue;

			Child->SetParent(this);
			Children.push_back(Child);

			const vec2f ChildScale = Child->GetWorldScale();
			const vec2f ChildSize = Child->GetSize().componentWiseMul(ChildScale);
			const vec2f ChildOrigin = Child->GetOrigin().componentWiseMul(ChildScale);

			// Left/top/right/bottom in local space
			const float Left = CursorX - ChildOrigin.x;
			const float Top = 0.f - ChildOrigin.y;
			const float Right = Left + ChildSize.x;
			const float Bottom = Top + ChildSize.y;

			MinX = std::min(MinX, Left);
			MaxX = std::max(MaxX, Right);
			MaxY = std::max(MaxY, Bottom);

			CursorX += ChildSize.x + Padding;
		}

		// ------------------------------------------------------------
		// HorizontalBox size = stacked width + tallest child
		// ------------------------------------------------------------

		const float ContentWidth = MaxX - MinX;
		const float ContentHeight = MaxY;

		const vec2f BoxSize = { ContentWidth, ContentHeight };
		SetSize(BoxSize);

		// ------------------------------------------------------------
		// Offset children so stacked content starts at top-left
		// ------------------------------------------------------------

		float ApplyX = 0.f;

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				const vec2f ChildScaledSize =
					Child->GetSize().componentWiseMul(Child->GetWorldScale());

				// Top-left aligned, only shift X
				const vec2f NewAnchorOffset =
					vec2f{ ApplyX - MinX, -0.f /*keep top*/ };

				Child->SetAnchorPosition(
					Anchor::TopLeft,
					Child->WidgetAnchor,
					NewAnchorOffset
				);

				ApplyX += ChildScaledSize.x + Padding;
			}
		}

		// ------------------------------------------------------------
		// Anchor the HorizontalBox itself
		// ------------------------------------------------------------

		SetAnchorPosition(
			InTargetAnchor,
			InWidgetAnchor,
			InOffset
		);
	}
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/VerticalBox.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/GameWindow.h"

namespace we
{
	/*VerticalBox::VerticalBox(EngineSubsystem& Subsystem, float InSpacing)
		: Widget{ Subsystem }, Spacing{ InSpacing }
	{
	}

	void VerticalBox::AddChild(shared<Widget> Child, Anchor InTargetAnchor, Anchor InWidgetAnchor, vec2f InOffset)
	{
		if (!Child || Child.get() == this) return;

		float YOffset = 0.f;
		for (auto& Weak : Children)
		{
			if (auto C = Weak.lock())
				YOffset += C->GetSize().y + Spacing;
		}

		Child->SetParent(this);
		Child->SetAnchorPosition(InTargetAnchor, InWidgetAnchor, { InOffset.x, InOffset.y + YOffset });
		Children.push_back(Child);
	}

	void VerticalBox::Update(float DeltaTime)
	{
	}

	void VerticalBox::Render(GameWindow& Window)
	{
		if (!IsVisible()) return;
		Widget::Render(Window);
	}*/
}
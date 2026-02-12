// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "UI/Widget/Widget.h"

namespace we
{
	class HorizontalBox : public Widget
	{
	public:
		HorizontalBox(
			EngineSubsystem& Subsystem,
			const list<shared<Widget>>& InChildren,
			float Padding = 10.f,
			Anchor InTargetAnchor = Anchor::TopLeft,
			Anchor InWidgetAnchor = Anchor::TopLeft,
			const vec2f& InOffset = { 0.f, 0.f }
		);
	};
}
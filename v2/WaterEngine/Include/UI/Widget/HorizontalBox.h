// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/Widget/Widget.h"

namespace we
{
	class HorizontalBox : public Widget
	{
	public:
		HorizontalBox(float InPadding = 10.f);

		void CollectRenderDepths(vector<RenderDepth>& OutDepths) const override;

		void AddChild(shared<Widget> Child, Anchor InTargetAnchor = Anchor::TopLeft, Anchor InWidgetAnchor = Anchor::TopLeft, vec2f InOffset = vec2f{ 0, 0 }) override;

		void SetPadding(float InPadding) { Padding = InPadding; RefreshLayout(); }
		float GetPadding() const { return Padding; }

		void RefreshLayout();

	private:
		float Padding = 10.f;
	};
}

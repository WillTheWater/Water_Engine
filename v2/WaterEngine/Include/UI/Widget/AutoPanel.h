// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/Widget/Widget.h"

namespace we
{
	enum class ChildAlignment
	{
		Left,
		Center,
		Right
	};

	class AutoPanel : public Widget
	{
	public:
		AutoPanel(
			color FillColor = color{ 50, 50, 50, 200 },
			color OutlineColor = color{ 100, 100, 100 },
			float OutlineThickness = 1.f,
			float InSpacing = 10.f);

		void CollectRenderDepths(vector<RenderDepth>& OutDepths) const override;
		void AddChild(shared<Widget> Child, ChildAlignment Align = ChildAlignment::Left, vec2f Offset = vec2f{ 0, 0 });

		void SetFillColor(color InColor);
		void SetOutlineColor(color InColor);
		void SetOutlineThickness(float Thickness);

		void SetBorder(float Left, float Right, float Top, float Bottom);
		void SetBorder(const rectf& Border);

		void SetSpacing(float InSpacing) { Spacing = InSpacing; RefreshLayout(); }
		float GetSpacing() const { return Spacing; }

		void RefreshLayout();

	private:
		mutable rectangle BackgroundRect;
		color FillColor{ 50, 50, 50, 200 };
		color OutlineColor{ 100, 100, 100 };
		float OutlineThickness = 1.f;
		float Spacing = 10.f;
		rectf Border{ {10.f, 10.f}, {10.f, 10.f} };

		vector<ChildAlignment> ChildAlignments;
		vector<vec2f> ChildOffsets;
	};
}

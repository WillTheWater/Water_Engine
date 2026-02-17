// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/GridBox.h"

namespace we
{
	GridBox::GridBox(uint InColumns, float InPadding)
		: NumColumns(InColumns)
		, Padding(InPadding)
	{
	}

	void GridBox::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				Child->CollectRenderDepths(OutDepths);
			}
		}
	}

	void GridBox::AddChild(shared<Widget> Child, Anchor InTargetAnchor, Anchor InWidgetAnchor, vec2f InOffset)
	{
		Widget::AddChild(Child, InTargetAnchor, InWidgetAnchor, InOffset);
		RefreshLayout();
	}

	void GridBox::RefreshLayout()
	{
		if (NumColumns == 0 || Children.empty()) return;

		vector<float> ColumnWidths(NumColumns, 0.f);
		vector<float> RowHeights;

		uint CurrentCol = 0;
		uint CurrentRow = 0;

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				vec2f ChildSize = Child->GetSize();
				ColumnWidths[CurrentCol] = std::max(ColumnWidths[CurrentCol], ChildSize.x);

				if (CurrentRow >= RowHeights.size())
				{
					RowHeights.push_back(0.f);
				}
				RowHeights[CurrentRow] = std::max(RowHeights[CurrentRow], ChildSize.y);

				CurrentCol++;
				if (CurrentCol >= NumColumns)
				{
					CurrentCol = 0;
					CurrentRow++;
				}
			}
		}

		CurrentCol = 0;
		CurrentRow = 0;
		float RowY = 0.f;

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				float ColX = 0.f;
				for (uint c = 0; c < CurrentCol; ++c)
				{
					ColX += ColumnWidths[c] + Padding;
				}

				Child->SetAnchorPosition(Anchor::TopLeft, Anchor::TopLeft, { ColX, RowY });

				CurrentCol++;
				if (CurrentCol >= NumColumns)
				{
					CurrentCol = 0;
					RowY += RowHeights[CurrentRow] + Padding;
					CurrentRow++;
				}
			}
		}

		float TotalWidth = 0.f;
		for (float w : ColumnWidths) TotalWidth += w;
		TotalWidth += (ColumnWidths.size() - 1) * Padding;

		float TotalHeight = 0.f;
		for (float h : RowHeights) TotalHeight += h;
		TotalHeight += (RowHeights.size() - 1) * Padding;

		SetSize({ TotalWidth, TotalHeight });
	}
}

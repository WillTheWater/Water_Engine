#include "UI/Widget/GridBox.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/GameWindow.h"

namespace we
{
	GridBox::GridBox(
		EngineSubsystem& Subsystem,
		const list<shared<Widget>>& InChildren,
		uint Columns,
		float Padding,
		Anchor InTargetAnchor,
		Anchor InWidgetAnchor,
		const vec2f& InOffset
	) : Widget{ Subsystem }, NumColumns{ Columns }, CellPadding{ Padding }
	{
		if (Columns == 0) Columns = 1;

		// Track max width per column and max height per row
		list<float> ColumnWidths(Columns, 0.f);
		list<float> RowHeights;

		uint CurrentColumn = 0;
		float CurrentRowMaxHeight = 0.f;
		uint ChildIndex = 0;

		for (const auto& Child : InChildren)
		{
			if (!Child) continue;

			Child->SetParent(this);
			Children.push_back(Child);

			const vec2f ChildSize = Child->GetSize().componentWiseMul(Child->GetWorldScale());

			// Update column width
			ColumnWidths[CurrentColumn] = std::max(ColumnWidths[CurrentColumn], ChildSize.x);

			// Track max height for this row
			CurrentRowMaxHeight = std::max(CurrentRowMaxHeight, ChildSize.y);

			CurrentColumn++;
			ChildIndex++;

			// End of row
			if (CurrentColumn >= Columns)
			{
				RowHeights.push_back(CurrentRowMaxHeight);
				CurrentColumn = 0;
				CurrentRowMaxHeight = 0.f;
			}
		}

		// Push the last row if not complete
		if (CurrentColumn != 0)
			RowHeights.push_back(CurrentRowMaxHeight);

		// ------------------------------------------------------------
		// Position children top-left, respecting padding
		// ------------------------------------------------------------

		std::vector<float> ColumnXOffsets(Columns, 0.f);
		for (uint i = 1; i < Columns; ++i)
			ColumnXOffsets[i] = ColumnXOffsets[i - 1] + ColumnWidths[i - 1] + CellPadding;

		std::vector<float> RowYOffsets(RowHeights.size(), 0.f);
		for (size_t r = 1; r < RowHeights.size(); ++r)
			RowYOffsets[r] = RowYOffsets[r - 1] + RowHeights[r - 1] + CellPadding;

		CurrentColumn = 0;
		size_t CurrentRow = 0;
		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				vec2f Pos = { ColumnXOffsets[CurrentColumn], RowYOffsets[CurrentRow] };
				Child->SetAnchorPosition(Anchor::TopLeft, Child->WidgetAnchor, Pos);
			}

			CurrentColumn++;
			if (CurrentColumn >= Columns)
			{
				CurrentColumn = 0;
				CurrentRow++;
			}
		}

		// ------------------------------------------------------------
		// Set size of GridBox = total width + total height + paddings
		// ------------------------------------------------------------

		float Width = 0.f;
		for (auto w : ColumnWidths) Width += w;
		Width += (Columns - 1) * CellPadding;

		float Height = 0.f;
		for (auto h : RowHeights) Height += h;
		Height += (RowHeights.size() - 1) * CellPadding;

		SetSize({ Width, Height });

		SetAnchorPosition(InTargetAnchor, InWidgetAnchor, InOffset);
	}
}

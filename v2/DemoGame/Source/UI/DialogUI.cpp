// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/DialogUI.h"
#include "UI/UIStyle.h"
#include "Subsystem/GuiSubsystem.h"

namespace we
{
	DialogUI::DialogUI() = default;

	DialogUI::~DialogUI()
	{
		if (Background)
		{
			MakeGUI().GetWorldUI().remove(Background);
		}
	}

	void DialogUI::Initialize()
	{
		if (bInitialized)
			return;

		// Black background with white border
		Background = UIStyle::CreatePanel({Width, Height});
		Background->getRenderer()->setBackgroundColor({0, 0, 0, 200});

		// Text label
		TextLabel = UIStyle::CreateLabel("", UILabelStyle::Small);
		TextLabel->setPosition({10.0f, 10.0f});
		TextLabel->setSize({Width - 20.0f, Height - 40.0f});
		TextLabel->setAutoSize(false);

		// Continue arrow (bottom right)
		ContinueArrow = UIStyle::CreateLabel("▼", UILabelStyle::Tiny);
		ContinueArrow->setPosition({Width - 25.0f, Height - 25.0f});
		ContinueArrow->setSize({20.0f, 20.0f});

		Background->add(TextLabel);
		Background->add(ContinueArrow);
		MakeGUI().GetWorldUI().add(Background);

		Background->setOrigin(Anchor.x, Anchor.y);
		Background->setVisible(false);

		bInitialized = true;
	}

	void DialogUI::Show()
	{
		if (Background && bInitialized)
		{
			Background->setVisible(true);
		}
	}

	void DialogUI::Hide()
	{
		if (Background && bInitialized)
		{
			Background->setVisible(false);
		}
	}

	bool DialogUI::IsVisible() const
	{
		return Background && Background->isVisible();
	}

	void DialogUI::SetDialog(const vector<string>& Lines)
	{
		DialogLines = Lines;
		CurrentLine = 0;
		DisplayCurrentLine();
	}

	bool DialogUI::Advance()
	{
		if (CurrentLine + 1 < DialogLines.size())
		{
			CurrentLine++;
			DisplayCurrentLine();
			return true;
		}
		return false;
	}

	bool DialogUI::HasMoreLines() const
	{
		return CurrentLine + 1 < DialogLines.size();
	}

	void DialogUI::DisplayCurrentLine()
	{
		if (TextLabel && CurrentLine < DialogLines.size())
		{
			TextLabel->setText(DialogLines[CurrentLine]);
		}
		
		// Hide continue arrow if on last line
		if (ContinueArrow)
		{
			ContinueArrow->setVisible(HasMoreLines());
		}
	}

	void DialogUI::SetPosition(const vec2f& WorldPos, const vec2f& Offset)
	{
		if (Background)
		{
			vec2f FinalPos = WorldPos + Offset;
			MakeGUI().SetWidgetWorldPosition(Background, FinalPos);
		}
	}

	void DialogUI::SetAnchor(const vec2f& AnchorPoint)
	{
		Anchor = AnchorPoint;
		if (Background)
		{
			Background->setOrigin(Anchor.x, Anchor.y);
		}
	}
}

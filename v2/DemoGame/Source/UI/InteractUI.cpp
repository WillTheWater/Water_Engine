// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/InteractUI.h"
#include "Subsystem/GuiSubsystem.h"

namespace we
{
	InteractUI::InteractUI() = default;

	InteractUI::~InteractUI()
	{
		if (Background)
		{
			MakeGUI().GetWorldUI().remove(Background);
		}
	}

	void InteractUI::Initialize(const string& Text)
	{
		if (bInitialized)
			return;

		// Black background with white border
		Background = tgui::Panel::create({Width, Height});
		Background->getRenderer()->setBackgroundColor({0, 0, 0, 200});
		Background->getRenderer()->setBorderColor({255, 255, 255, 255});
		Background->getRenderer()->setBorders({2, 2, 2, 2});

		// White centered text
		TextLabel = tgui::Label::create(Text);
		TextLabel->setTextSize(14);
		TextLabel->getRenderer()->setTextColor({255, 255, 255, 255});
		TextLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
		TextLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
		TextLabel->setSize({Width, Height});

		Background->add(TextLabel);
		MakeGUI().GetWorldUI().add(Background);

		Background->setOrigin(Anchor.x, Anchor.y);
		Background->setVisible(false);

		bInitialized = true;
	}

	void InteractUI::Show()
	{
		if (Background && bInitialized)
		{
			Background->setVisible(true);
		}
	}

	void InteractUI::Hide()
	{
		if (Background && bInitialized)
		{
			Background->setVisible(false);
		}
	}

	bool InteractUI::IsVisible() const
	{
		return Background && Background->isVisible();
	}

	void InteractUI::SetPosition(const vec2f& ScreenPos)
	{
		if (Background)
		{
			Background->setPosition({ScreenPos.x, ScreenPos.y});
		}
	}

	void InteractUI::SetText(const string& Text)
	{
		if (TextLabel)
		{
			TextLabel->setText(Text);
		}
	}

	void InteractUI::SetAnchor(const vec2f& AnchorPoint)
	{
		Anchor = AnchorPoint;
		if (Background)
		{
			Background->setOrigin(Anchor.x, Anchor.y);
		}
	}
}
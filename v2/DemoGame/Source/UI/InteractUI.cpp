// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/InteractUI.h"
#include "Subsystem/GuiSubsystem.h"
#include "Utility/Log.h"

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

		Background = tgui::Panel::create({Width, Height});
		Background->getRenderer()->setBackgroundColor({255, 0, 0, 255});
		Background->getRenderer()->setBorderColor({255, 255, 255, 200});
		Background->getRenderer()->setBorders({2, 2, 2, 2});

		TextLabel = tgui::Label::create(Text);
		TextLabel->setTextSize(14);
		TextLabel->getRenderer()->setTextColor({255, 255, 255});
		TextLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
		TextLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
		TextLabel->setSize({Width, Height});

		Background->add(TextLabel);

		MakeGUI().GetWorldUI().add(Background);

		bInitialized = true;
	}

	void InteractUI::Show()
	{
		if (!bInitialized || bVisible)
			return;

		if (Background)
		{
			Background->setVisible(true);
		}
		bVisible = true;
	}

	void InteractUI::Hide()
	{
		if (!bInitialized || !bVisible)
			return;

		if (Background)
		{
			Background->setVisible(false);
		}
		bVisible = false;
	}

	void InteractUI::SetPosition(const vec2f& WorldPos)
	{
		if (Background)
		{
			Background->setPosition({WorldPos.x, WorldPos.y});
		}
	}

	void InteractUI::SetText(const string& Text)
	{
		if (TextLabel)
		{
			TextLabel->setText(Text);
		}
	}
}
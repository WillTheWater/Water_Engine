// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GuiSubsystem.h"
#include "EventHandler/GUIEventHandler.h"
#include "Utility/Log.h"

namespace we
{
	GUISubsystem* GUISubsystem::Instance = nullptr;

	GUISubsystem::GUISubsystem()
	{
		Instance = this;
	}

	GUISubsystem& GUISubsystem::Get()
	{
		return *Instance;
	}

	void GUISubsystem::Initialize(renderTarget& ScreenUITarget, renderTarget& WorldUITarget)
	{
		ScreenUI.setTarget(ScreenUITarget);
		WorldUI.setTarget(WorldUITarget);
	}

	bool GUISubsystem::HandleEvent(const sf::Event& event)
	{
		GUIEventHandler Handler{*this};
		event.visit(Handler);
		return Handler.Consumed;
	}

	bool GUISubsystem::ShouldBlockInput() const
	{
		tgui::Vector2i mousePos = ScreenUI.getLastMousePosition();
		
		// Check ScreenUI
		auto screenWidget = ScreenUI.getWidgetBelowMouseCursor(mousePos, true);
		if (screenWidget)
		{
			LOG("[ShouldBlockInput] Blocking - ScreenUI widget: {}", screenWidget->getWidgetName().toStdString());
			return true;
		}
		
		// Check WorldUI
		auto worldWidget = WorldUI.getWidgetBelowMouseCursor(mousePos, true);
		if (worldWidget)
		{
			LOG("[ShouldBlockInput] Blocking - WorldUI widget: {}", worldWidget->getWidgetName().toStdString());
			return true;
		}
		return false;
	}
}

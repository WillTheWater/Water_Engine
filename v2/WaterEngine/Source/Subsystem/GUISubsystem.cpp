// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GuiSubsystem.h"
#include "Subsystem/CameraSubsystem.h"
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

	void GUISubsystem::SetCameraView(const view NewView)
	{		
		tgui::FloatRect ViewRect{
			0,
			0,
			NewView.getSize().x,
			NewView.getSize().y
		};
		
		ScreenUI.setAbsoluteView(ViewRect);
		WorldUI.setAbsoluteView(ViewRect);
		rectf vp = NewView.getViewport();
		vp.size.x;
		auto NewThing = tgui::FloatRect({vp.position.x, vp.position.y}, {vp.size.x, vp.size.y});
		ScreenUI.setAbsoluteViewport(NewThing);
		WorldUI.setAbsoluteViewport(NewThing);
	}

	bool GUISubsystem::HandleEvent(const sf::Event& event)
	{
		GUIEventHandler Handler{*this};
		event.visit(Handler);
		return Handler.Consumed;
	}
}
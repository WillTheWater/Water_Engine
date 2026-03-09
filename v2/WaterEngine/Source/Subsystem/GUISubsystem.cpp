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

	void GUISubsystem::SetCameraView(const view& NewView)
	{		
		tgui::FloatRect ViewRect{0, 0, 1920, 1080};
		
		ScreenUI.setAbsoluteView(ViewRect);
		WorldUI.setAbsoluteView(ViewRect);

		rectf vp = NewView.getViewport();
		auto TguiVP = tgui::FloatRect({vp.position.x, vp.position.y}, {vp.size.x, vp.size.y});
		ScreenUI.setRelativeViewport(TguiVP);
		WorldUI.setRelativeViewport(TguiVP);
	}

	bool GUISubsystem::HandleEvent(const event& event)
	{
		GUIEventHandler Handler{*this};
		event.visit(Handler);
		return Handler.Consumed;
	}
}
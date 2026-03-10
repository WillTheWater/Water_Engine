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
		: WindowSize{1920, 1080}
		, CurrentCameraView{}
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

	void GUISubsystem::SetWindowSize(vec2u NewSize)
	{
		WindowSize = NewSize;
	}

	void GUISubsystem::SetCameraView(const view& NewView)
	{
		CurrentCameraView = NewView;
	}

	bool GUISubsystem::HandleEvent(const event& Event)
	{
		GUIEventHandler Handler{*this};
		Event.visit(Handler);
		return Handler.Consumed;
	}
}
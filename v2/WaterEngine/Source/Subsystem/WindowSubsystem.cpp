// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WindowSubsystem.h"
#include "Core/EngineConfig.h"

namespace we
{
	WindowSubsystem::WindowSubsystem()
	{
		create(window(WEConfig.Window.DefaultWindowSize), WEConfig.Window.WindowTitle);
		setVerticalSyncEnabled(WEConfig.Window.bVSyncEnabled);
		setMouseCursorVisible(WEConfig.Window.bMouseCursorVisible);
		setKeyRepeatEnabled(WEConfig.Window.bKeyRepeatEnabled);
	}

	void WindowSubsystem::HandleEvent(const event& Event)
	{
		WindowEventHandler Handler{ *this };
		Event.visit(Handler);
	}

	void WindowSubsystem::EventResize()
	{
		OnResize.Broadcast(getSize());
	}

	void WindowSubsystem::EventMouseMoved(vec2f NewPosition)
	{
		OnMouseMove.Broadcast(NewPosition);
	}
}
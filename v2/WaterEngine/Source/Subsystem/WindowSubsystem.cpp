// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WindowSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	WindowSubsystem::WindowSubsystem()
	{
		create(window({ 1920, 1080 }), "Water Engine");
		setVerticalSyncEnabled(true);
		setMouseCursorVisible(false);
		setKeyRepeatEnabled(false);
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
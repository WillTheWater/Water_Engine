// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WindowSubsystem.h"

namespace we
{
	WindowSubsystem::WindowSubsystem()
	{
		create(window({ 1920, 1080 }), "Water Engine");
	}

	void WindowSubsystem::HandleEvent(const event& Event)
	{
		WindowEventHandler Handler{ *this };
		Event.visit(Handler);
	}
}
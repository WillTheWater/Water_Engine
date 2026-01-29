// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/RenderSubsystem.h"

namespace we
{
	struct EngineSubsystem
	{
		TimerSubsystem Time;
		RenderSubsystem Render;
	};
}
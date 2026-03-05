// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/ClockSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/CameraSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	struct EngineSubsystem
	{
		unique<ResourceSubsystem>	Resource;
		unique<WindowSubsystem>		Window;
		unique<ClockSubsystem>		Clock;
		unique<RenderSubsystem>		Render;
		unique<CameraSubsystem>		Camera;
	};
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Subsystem/WindowSubsystem.h"

namespace we
{
	struct EngineSubsystem
	{
		unique<WindowSubsystem> Window;
	};
}

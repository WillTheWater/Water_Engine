// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/SaveLoadSubsystem.h"
#include "Subsystem/AudioSubsystem.h"

namespace we
{
	struct EngineSubsystem
	{
		unique<TimerSubsystem> Time;
		unique<RenderSubsystem> Render;
		unique<SaveLoadSubsystem> SaveLoad;
		unique<AudioSubsystem> Audio;
	};
}
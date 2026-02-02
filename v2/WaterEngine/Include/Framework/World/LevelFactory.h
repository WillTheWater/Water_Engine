// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/World/Level.h"

namespace we
{
	namespace LF
	{
		using Levels = dictionary<string, unique<Level>>;

		Levels CreateLevels(EngineSubsystem& Subsystem);
	}
}
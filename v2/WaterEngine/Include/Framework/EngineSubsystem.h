// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

#include "Interface/IAssetDirector.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/CameraSubsystem.h"
#include "Subsystem/TimeSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/WindowSubsystem.h"

namespace we
{
	class GameInstance;

	struct EngineSubsystem
	{
		unique<ResourceSubsystem> Resources;
		unique<WorldSubsystem> Worlds;
		unique<CameraSubsystem> Camera;
		unique<TimeSubsystem> Time;
		unique<RenderSubsystem> Render;
		unique<WindowSubsystem> Window;
	};
}
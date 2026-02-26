// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

#include "Interface/IAssetDirector.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/WindowSubsystem.h"

namespace we
{
	class GameInstance;

	struct EngineSubsystem
	{
		unique<IAssetDirector> AssetDirectory;
		unique<RenderSubsystem> Render;
		unique<WindowSubsystem> Window;
	};
}
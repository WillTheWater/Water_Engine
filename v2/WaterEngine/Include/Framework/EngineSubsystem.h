// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/TimeSubsystem.h"
#include "UI/Cursor/CursorSubsystem.h"


#include "Subsystem/SaveLoadSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/GameStateSubsystem.h"
#include "Subsystem/GUISubsystem.h"

namespace we
{
	struct EngineSubsystem
	{
		unique<WindowSubsystem> Window;
		unique<RenderSubsystem> Render;
		unique<TimeSubsystem> Time;
		unique<CursorSubsystem> Cursor;



		// unique<SaveLoadSubsystem> SaveLoad;
		// unique<AudioSubsystem> Audio;
		// unique<InputSubsystem> Input;
		// unique<WorldSubsystem> World;
		// unique<GameStateSubsystem> GameState;
		// unique<GUISubsystem> GUI;
	};
}
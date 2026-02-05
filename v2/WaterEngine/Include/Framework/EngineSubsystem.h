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
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/GameStateSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "UI/Cursor/CursorSubsystem.h"

namespace we
{
	struct EngineSubsystem
	{
		unique<TimerSubsystem> Time;
		unique<RenderSubsystem> Render;
		unique<SaveLoadSubsystem> SaveLoad;
		unique<AudioSubsystem> Audio;
		unique<InputSubsystem> Input;
		unique<WorldSubsystem> World;
		unique<GameStateSubsystem> GameState;
		unique<GUISubsystem> GUI;
		unique<CursorSubsystem> Cursor;
	};
}
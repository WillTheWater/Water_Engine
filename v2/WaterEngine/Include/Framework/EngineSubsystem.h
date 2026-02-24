// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/TimeSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/SaveLoadSubsystem.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/GameStateSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Subsystem/CameraSubsystem.h"

namespace we
{
	class GameInstance;

	struct EngineSubsystem
	{
		unique<GameInstance> GameInst;
		unique<ResourceSubsystem> AssetLoader;
		unique<SaveLoadSubsystem> SaveLoad;
		unique<TimeSubsystem> Time;
		unique<InputSubsystem> Input;
		unique<CursorSubsystem> Cursor;
		unique<GameStateSubsystem> GameState;
		unique<AudioSubsystem> Audio;
		unique<PhysicsSubsystem> Physics;
		unique<WorldSubsystem> World;
		unique<GUISubsystem> GUI; 
		unique<RenderSubsystem> Render;
		unique<CameraSubsystem> Camera;
		unique<WindowSubsystem> Window;
	};
}
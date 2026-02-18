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

namespace we
{
	struct EngineSubsystem
	{
		// Dependencies: Declare AFTER things they depend on
		// (Destroyed in reverse order: things declared first are destroyed last)
		
		unique<ResourceSubsystem> AssetLoader;
		unique<SaveLoadSubsystem> SaveLoad;
		unique<TimeSubsystem> Time;
		unique<InputSubsystem> Input;
		unique<CursorSubsystem> Cursor;
		unique<GameStateSubsystem> GameState;
		unique<AudioSubsystem> Audio;
		unique<PhysicsSubsystem> Physics;  // Destroyed after World (World's actors need physics cleanup)
		unique<WorldSubsystem> World;      // Destroyed after GUI
		unique<GUISubsystem> GUI;          // Destroyed after Render/Audio
		unique<RenderSubsystem> Render;    // Destroyed after Window
		unique<WindowSubsystem> Window;    // Destroyed last (OS window must outlive everything)
	};
}
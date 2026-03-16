// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/ClockSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/CameraSubsystem.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Subsystem/SaveSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/PhysicsSubsystem.h"

namespace we
{
	struct EngineSubsystem
	{
		unique<ResourceSubsystem>	Resource;
		unique<AudioSubsystem>		Audio;
		unique<WindowSubsystem>		Window;
		unique<ClockSubsystem>		Clock;
		unique<TimerSubsystem>		Timer;
		unique<WorldSubsystem>		World;
		unique<RenderSubsystem>		Render;
		unique<CameraSubsystem>		Camera;
		unique<CursorSubsystem>		Cursor;
		unique<InputSubsystem>		Input;
		unique<GUISubsystem>		GUI;
		unique<SaveSubsystem>		Save;
		unique<PhysicsSubsystem>	Physics;
	};
}
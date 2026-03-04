// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

#include "Interface/IAssetDirector.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Framework/World/WorldFactory.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/CameraSubsystem.h"
#include "Subsystem/TimeSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Subsystem/CursorSubsystem.h"

namespace we
{
	struct EngineSubsystem
	{
		// Members destroyed in REVERSE declaration order.
		// If X uses Y during destruction, Y must be declared BEFORE X.
		
		unique<ResourceSubsystem>	Resources;
		
		unique<WindowSubsystem>		Window;
		unique<RenderSubsystem>		Render;
		
		// Physics used by Actor during destruction, so Physics must outlive World
		unique<PhysicsSubsystem>	Physics;
		unique<WorldSubsystem>		World;
		
		unique<CameraSubsystem>		Camera;
		unique<AudioSubsystem>		Audio;
		unique<InputSubsystem>		Input;
		unique<TimeSubsystem>		Time;
		unique<GUISubsystem>		GUI;
		unique<CursorSubsytem>		Cursor;
	};
}

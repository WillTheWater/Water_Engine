// =============================================================================
// Water Engine v2.1.1
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/GameInstance.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	void GameInstance::Init(EngineSubsystem& InSubsystem)
	{
		Subsystem = &InSubsystem;
	}

	void GameInstance::Shutdown()
	{
		Subsystem = nullptr;
	}
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "DemoGameInstance.h"
#include "Utility/Log.h"

namespace we
{
	DemoGameInstance::DemoGameInstance()
	{
		LOG("[DemoGameInstance] Created");
	}

	void DemoGameInstance::Init(EngineSubsystem& InSubsystem)
	{
		GameInstance::Init(InSubsystem);
		LOG("[DemoGameInstance] Initialized");
	}

	void DemoGameInstance::Shutdown()
	{
		LOG("[DemoGameInstance] Shutdown");
		GameInstance::Shutdown();
	}
}

// =============================================================================
// Water Engine v2.1.2
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "DemoGameInstance.h"
#include "Utility/Log.h"

namespace we
{
	DemoGameInstance::DemoGameInstance()
	{
	}

	void DemoGameInstance::Init(EngineSubsystem& InSubsystem)
	{
		GameInstance::Init(InSubsystem);
	}

	void DemoGameInstance::Shutdown()
	{
		GameInstance::Shutdown();
	}
}
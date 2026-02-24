// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#include "Framework/GameInstance.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	GameInstance::GameInstance() = default;
	GameInstance::~GameInstance() = default;

	void GameInstance::Init(EngineSubsystem& InSubsystem)
	{
		Subsystem = &InSubsystem;
		bInitialized = true;
	}

	void GameInstance::Shutdown()
	{
		bInitialized = false;
		Subsystem = nullptr;
	}

	void GameInstance::Tick(float DeltaTime)
	{
		// Rare Use Cases
	}
}

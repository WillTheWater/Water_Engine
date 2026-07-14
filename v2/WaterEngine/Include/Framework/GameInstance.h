// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	struct EngineSubsystem;

	// =============================================================================
	// GameInstance
	// =============================================================================
	
	class GameInstance
	{
	public:
		GameInstance() = default;
		virtual ~GameInstance() = default;

		// Called once after construction, before game loop begins
		virtual void Init(EngineSubsystem& InSubsystem);

		// Called once before destruction
		virtual void Shutdown();

		// Called every frame from WaterEngine::Update().
		virtual void Tick(float /*DeltaTime*/) {}

	protected:
		EngineSubsystem* Subsystem = nullptr;
	};
}
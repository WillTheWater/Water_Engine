// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	struct EngineSubsystem;

	// =============================================================================
	// GameInstance
	// =============================================================================
	// Base class for persistent game-wide data and systems.
	// Lifetime: Created before Initialize(), destroyed after shutdown.
	// Access via: Subsystem.GameInstance
	// =============================================================================
	
	class GameInstance
	{
	public:
		GameInstance();
		virtual ~GameInstance();

		// Called once after construction, before the game loop begins
		virtual void Init(EngineSubsystem& InSubsystem);

		// Called once when the game is shutting down, before destruction
		virtual void Shutdown();

		// Called every frame (use sparingly - prefer subsystems for tick logic)
		virtual void Tick(float DeltaTime);

		// Access to engine subsystems
		EngineSubsystem& GetSubsystem() const { return *Subsystem; }

	protected:
		EngineSubsystem* Subsystem = nullptr;
		bool bInitialized = false;
	};
}

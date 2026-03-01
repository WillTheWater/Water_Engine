// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"

namespace we
{
	class MainMenu : public World
	{
	public:
		MainMenu(EngineSubsystem& Subsystem);

		// World overrides
		virtual void PreConstruct() override;
		virtual void BeginPlay() override;
		virtual void EndPlay() override;
		virtual void Tick(float DeltaTime) override;

	private:
		// =============================================================================
		// INPUT SYSTEM TESTS - REMOVE AFTER TESTING
		// =============================================================================
		void SetupInputTests();  // Binds all test inputs
		void CleanupInputTests(); // Unbinds test inputs
		
		// Test state tracking
		bool bTestJumpTriggered = false;
		bool bTestFireHeld = false;
		float TestMovementX = 0.0f;
		float TestMovementY = 0.0f;
		// END INPUT TESTS

	private:
		weak<Actor> Background;
		weak<Actor> TitleCard;
		weak<Actor> PhysicsBox;  // Track the falling box for debug
	};
}

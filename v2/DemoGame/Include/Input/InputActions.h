// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

// =============================================================================
// Game Input Actions - TEST DEFINITIONS
// These are for testing the InputSubsystem - REMOVE AFTER TESTING
// =============================================================================

enum InputActions : int
{
	// Movement (State-based testing)
	TEST_MOVE_UP = 1,
	TEST_MOVE_DOWN,
	TEST_MOVE_LEFT,
	TEST_MOVE_RIGHT,
	
	// Event-based testing
	TEST_ACTION_JUMP,        // OnPressed test
	TEST_ACTION_FIRE,        // OnHeld test
	TEST_ACTION_INTERACT,    // OnReleased test
	
	// Gamepad button tests
	TEST_GAMEPAD_SOUTH,      // A button (Xbox) / X (PlayStation)
	TEST_GAMEPAD_EAST,       // B button (Xbox) / Circle (PlayStation)
	TEST_GAMEPAD_START,      // Start/Menu button
	
	// GUI Navigation (for menu testing)
	TEST_GUI_CONFIRM,
	TEST_GUI_CANCEL,
};

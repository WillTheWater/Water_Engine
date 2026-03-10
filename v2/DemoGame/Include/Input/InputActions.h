// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

// =============================================================================
// Game Input Actions
// =============================================================================

enum InputActions : int
{
	// Movement
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	
	ACTION_JUMP,        // OnPressed test
	ACTION_FIRE,        // OnHeld test
	ACTION_INTERACT,    // OnReleased test
	
	// Gamepad button tests
	GAMEPAD_SOUTH,  
	GAMEPAD_EAST,     
	GAMEPAD_START,
	
	PAUSE_ACTION
};

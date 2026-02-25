// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

// =============================================================================
// Game Input Actions
// =============================================================================

enum InputActions : int
{
	// Movement (used by MovementComponent)
	MOVE_UP = 1,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	
	// Game Actions
	ACTION_INTERACT,       // E key / Gamepad South - interact with NPCs/objects
	ACTION_TOGGLE_PAUSE,   // ESC / Gamepad Start - pause menu
	
	// GUI Navigation Actions
	GUI_CONFIRM,           // Gamepad South button (A on Xbox) - press/click
	GUI_CANCEL,            // Gamepad East button (B on Xbox) - back/cancel
	GUI_NAV_NEXT,          // Right shoulder - next widget
	GUI_NAV_PREVIOUS,      // Left shoulder - previous widget
};

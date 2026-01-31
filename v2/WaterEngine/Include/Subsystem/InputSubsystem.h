// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Input/InputBinding.h"
#include <SFML/Window/Event.hpp>

namespace we
{
/*
 *
 *                              InputSubsystem
 *
 * Supports three input modalities:
 *   1. State-Based (Polling): Check current hardware state every frame via IsPressed()
 *   2. Event-Based (One-Shot): Callbacks triggered immediately by SFML state transitions
 *   3. Held Callbacks: Continuous callbacks firing every frame while input is maintained
 *
 * Design Philosophy:
 *   - Actions are identified by integer IDs (game-specific enums)
 *   - Multiple physical inputs can bind to the same action (OR logic: Spacebar OR Gamepad South = Jump)
 *   - Decouples hardware timing (OS events) from game logic tick (update loop)
 *
 * -----------------------------------------------------------------------------
 * CALLBACKS vs QUERIES: Architectural Distinction
 * -----------------------------------------------------------------------------
 *
 *  OnPressed/OnReleased (Callbacks -- Push Pattern)
 *  - Execution: Immediate, during HandleEvent() when SFML event arrives (mid-frame)
 *  - Timing: Lowest possible latency from hardware to reaction
 *  - Best for: UI effects, audio feedback, window management, stateless reactions
 *  - Limitation: Executes before game logic tick; cannot access current game state
 *
 *  IsJustPressed/IsJustReleased (Queries -- Pull Pattern)
 *  - Execution: Deferred, during game logic tick when explicitly checked
 *  - Timing: End-of-frame, consistent with game state evaluation
 *  - Best for: Gameplay actions requiring validation (CanJump(), HasAmmo(), !IsPaused())
 *  - Advantage: Conditional logic centralized in game systems, not scattered in setup
 *
 *  Frame Timeline:
 *      ProcessEvents() -> HandleEvent() -> [OnPressed Callbacks Fire]
 *      GlobalTick() -> [IsJustPressed Checked] -> Game Logic Evaluates Conditions
 *      PostUpdate() -> Frame Counter Advances -> [IsJustPressed Returns False]
 *
 * Recommendation:
 *   - Use Callbacks (OnPressed) for immediate feedback that MUST happen instantly
 *     (button click sounds, UI hover effects, visual flourishes)
 *   - Use Queries (IsJustPressed) for gameplay mechanics requiring state validation
 *     (jumping, firing, reloading where CanInteract() checks are required)
 *
 * -----------------------------------------------------------------------------
 * Usage Example:
 * -----------------------------------------------------------------------------
 *
 *   // Setup (GameMode/PlayerController initialization)
 *   Input.Bind(ACTION_JUMP, Input::Keyboard{sf::Keyboard::Scan::Space});
 *   Input.Bind(ACTION_JUMP, Input::Gamepad{GamepadButton::South, 0});
 *
 *   // Pattern A: Callback for immediate stateless response (audio/visual)
 *   Input.OnPressed(ACTION_JUMP, [&](){
 *       audio.Play("jump_start.wav");  // Always play sound, even if jump fails
 *   });
 *
 *   // Pattern B: Held callback for continuous actions (fire rate, charging)
 *   Input.OnHeld(ACTION_FIRE, [&](){
 *       weapon.Fire();  // Called every frame (60-144Hz) while trigger held
 *   });
 *
 *   // Game Loop
 *   while (running) {
 *       // Poll window events (triggers OnPressed/OnReleased callbacks immediately)
 *       engine.ProcessEvents();  // Calls Input.HandleEvent() internally
 *
 *       // Per-frame polling for analog/continuous movement (smooth interpolation)
 *       if (Input.IsPressed(ACTION_MOVE_LEFT))
 *           player.velocity.x = -speed;
 *
 *       // Pattern C: Query with game-state validation (conditional execution)
 *       if (Input.IsJustPressed(ACTION_JUMP) && player.CanJump() && !player.IsStunned())
 *       {
 *           player.Jump();  // Only jumps if game logic allows
 *       }
 *
 *       engine.PostUpdate();  // Advances frame counter, clearing IsJust states
 *   }
 *
 * Thread Safety:
 *   - All public methods must be called from the main thread only
 *   - SFML window events are inherently single-threaded
 */

	class InputSubsystem
	{
	public:
		// ============ STATE-BASED API ============
		void Bind(int InputAction, const Input::Binding& Binding);
		bool IsPressed(int InputAction) const;
		void UnbindAll();

		// ============ EVENT-BASED API ============

		void HandleEvent(const sf::Event& Event);

		bool IsJustPressed(int InputAction) const;
		bool IsJustReleased(int InputAction) const;

		using ActionCallback = std::function<void()>;
		void OnPressed(int InputAction, ActionCallback Callback);
		void OnReleased(int InputAction, ActionCallback Callback);
		void OnHeld(int InputAction, ActionCallback Callback);

		void PostUpdate();

		void ProcessPressed(const Input::Binding& Binding);
		void ProcessReleased(const Input::Binding& Binding);
		void ProcessHeld();

	private:
		library<int, Input::Binding> InputBindings;

		uint64_t CurrentFrame = 0;
		dictionary<int, uint64_t> PressedOnFrame;
		dictionary<int, uint64_t> ReleasedOnFrame;
		dictionary<int, std::vector<ActionCallback>> HeldCallbacks;

		dictionary<int, std::vector<ActionCallback>> PressedCallbacks;
		dictionary<int, std::vector<ActionCallback>> ReleasedCallbacks;

		bool IsPressed(const Input::Keyboard& Binding) const;
		bool IsPressed(const Input::Mouse& Binding) const;
		bool IsPressed(const Input::Gamepad& Binding) const;
		bool IsPressed(const Input::JoystickAxis& Binding) const;
	};
}
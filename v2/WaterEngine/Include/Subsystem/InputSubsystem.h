// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Input/InputBinding.h"
#include <SFML/Window/Event.hpp>


/* =============================================================================
 *
 *                              InputSubsystem
 * 
 * =============================================================================
 *
 * Supports three input types:
 *   1. State-Based (Polling): Check current hardware state every frame via IsPressed()
 *   2. Event-Based (One-Shot): Callbacks triggered immediately by SFML state transitions
 *   3. Held Callbacks: Continuous callbacks firing every frame while input is maintained
 *
 * =============================================================================
 * CALLBACKS vs QUERIES
 * =============================================================================
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
 * =============================================================================
 * INPUT USAGE GUIDE
 * =============================================================================
 *
 * STEP 1: Define Input Actions
 * ----------------------------
 *
 *     enum InputAction
 *     {
 *         ACTION_JUMP,
 *         ACTION_FIRE,
 *         ACTION_MOVE_LEFT,
 *         ACTION_PAUSE
 *     };
 *
 *
 * STEP 2: Bind Hardware to Actions (Setup Phase)
 * ----------------------------------------------
 * In Game::BindInput() or Player::BeginPlay():
 *
 *     // Multiple bindings per action (OR logic)
 *     Input->Bind(ACTION_JUMP, Input::Keyboard{sf::Keyboard::Scan::Space});
 *     Input->Bind(ACTION_JUMP, Input::Gamepad{GamepadButton::South, 0});  // 0 = Player 1
 *     Input->Bind(ACTION_FIRE, Input::Mouse{sf::Mouse::Button::Left});
 *
 *
 * STEP 3: Choose Response Pattern
 * -------------------------------
 *
 * +-----------------+------------------+-----------------+--------------------+
 * | Pattern         | When It Fires    | Fires Paused?   | Best For           |
 * +-----------------+------------------+-----------------+--------------------+
 * | OnPressed       | Once per press   | YES             | Toggle, UI, pause  |
 * | OnHeld          | Every tick       | NO              | Auto-fire, charge  |
 * | IsPressed       | Every tick       | NO              | Movement           |
 * | IsJustPressed   | One frame only   | NO              | Abilities, jump    |
 * +-----------------+------------------+-----------------+--------------------+
 *
 *
 * PATTERN A: OnPressed Callback - Toggle/Immediate Response
 * ---------------------------------------------------------
 * Use for: Pause, UI clicks, actions that must work when game is paused
 *
 *     Input->OnPressed(ACTION_PAUSE, [this]()
 *     {
 *         // Fires ONCE per key press, holding does NOT repeat
 *         // Fires during ProcessEvents() - runs even when paused!
 *         TogglePause();
 *     });
 *
 *
 * PATTERN B: OnHeld Callback - Continuous Action
 * ----------------------------------------------
 * Use for: Automatic weapons, charging, sustained effects
 *
 *     Input->OnHeld(ACTION_FIRE, [this]()
 *     {
 *         // Called every tick while button held
 *         // ONLY runs when game is NOT paused
 *         Weapon->Fire();
 *     });
 *
 *
 * PATTERN C: Query - Validated Gameplay Action
 * --------------------------------------------
 * Use for: Movement, abilities that need state checks
 *
 *     void Player::Tick(float DeltaTime)
 *     {
 *         // Smooth movement (polled every frame)
 *         // ONLY runs when game is NOT paused
 *         if (Input->IsPressed(ACTION_MOVE_LEFT))
 *             Velocity.x = -Speed;
 *
 *         // One-shot with validation (true for ONE frame only)
 *         if (Input->IsJustPressed(ACTION_JUMP))
 *         {
 *             if (CanJump() && !IsStunned())  // State checks OK here
 *                 Jump();
 *         }
 *     }
 *
 * FRAME TIMELINE
 * --------------
 *     ProcessEvents()  -> OnPressed/OnReleased fire (ALWAYS runs)
 *           |
 *     [Pause Check]
 *           |
 *     Tick()           -> IsPressed/IsJustPressed/OnHeld (skipped if paused)
 *           |
 *     PostUpdate()     -> IsJustPressed resets for next frame
 *
 *
 * RULE OF THUMB
 * -------------
 *     - Use OnPressed for actions that must work while paused (UI, pause menu)
 *     - Use OnHeld/IsPressed/IsJustPressed for gameplay (skipped when paused)
 *     - OnPressed fires ONCE per press, does NOT auto-repeat when held
 *     - Never mix callback + query for same action (double response)
 *
 * ============================================================================= */

namespace we
{
	class InputSubsystem
	{
	public:
		InputSubsystem();

		static InputSubsystem& Get();

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

		float GetAxisValue(int JoystickID, sf::Joystick::Axis Axis) const;

	private:
		static InputSubsystem* Instance;

		library<int, Input::Binding> InputBindings;

		dictionary<ulong, vector<int>> BindingToActions;
		dictionary<int, int64> PressedOnFrame;
		dictionary<int, int64> ReleasedOnFrame;
		int64 CurrentFrame = 0;

		dictionary<int, vector<ActionCallback>> HeldCallbacks;
		dictionary<int, vector<ActionCallback>> PressedCallbacks;
		dictionary<int, vector<ActionCallback>> ReleasedCallbacks;

	private:
		bool IsPressed(const Input::Keyboard& Binding) const;
		bool IsPressed(const Input::Mouse& Binding) const;
		bool IsPressed(const Input::Gamepad& Binding) const;
		bool IsPressed(const Input::JoystickAxis& Binding) const;
	};

	inline InputSubsystem& InputController() { return InputSubsystem::Get(); }

	struct BindingHash 
	{
		ulong operator()(const Input::Binding& b) const 
		{
			return std::visit([](const auto& v) -> ulong
			{
				using T = std::decay_t<decltype(v)>;
				if constexpr (std::is_same_v<T, Input::Keyboard>)
				{
					return std::hash<int>{}(static_cast<int>(v.Key));
				}
				else if constexpr (std::is_same_v<T, Input::Mouse>) 
				{
					return std::hash<int>{}(static_cast<int>(v.Button)) ^ 0x1000;
				}
				else if constexpr (std::is_same_v<T, Input::Gamepad>)
				{
					return std::hash<int>{}(static_cast<int>(v.Button))
						^ (std::hash<int>{}(v.GamepadID) << 8)
						^ 0x2000;
				}
				else if constexpr (std::is_same_v<T, Input::JoystickAxis>)
				{
					return std::hash<int>{}(static_cast<int>(v.Axis))
						^ (std::hash<int>{}(v.AxisID) << 8)
						^ 0x3000;
				}
			}, b);
		}
	};
}
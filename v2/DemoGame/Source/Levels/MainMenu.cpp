// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Framework/EngineSubsystem.h"
#include "Input/InputActions.h"
#include "Input/InputBinding.h"
#include "Utility/Log.h"

namespace we
{
	MainMenu::MainMenu(EngineSubsystem& Subsystem)
		: World(Subsystem)
	{
	}

	void MainMenu::PreConstruct()
	{
		// Create background rectangle with custom render depth (drawn behind everything)
		auto BgActor = SpawnActor<Actor>();
		if (auto bg = BgActor.lock())
		{
			bg->SetAsRectangle({ 1920.0f, 1080.0f }, color(20, 30, 50));
			bg->SetPosition({ 960.0f, 540.0f });
			bg->SetCustomRenderDepth(-1000.0f);
			Background = bg;
		}

		// Create title card
		auto TitleActor = SpawnActor<Actor>();
		if (auto title = TitleActor.lock())
		{
			title->SetAsRectangle({ 400.0f, 150.0f }, color(200, 180, 100));
			title->SetPosition({ 960.0f, 300.0f });
			TitleCard = title;
		}

		// Create some floating shapes
		for (int i = 0; i < 5; ++i)
		{
			auto ShapeActor = SpawnActor<Actor>();
			if (auto shape = ShapeActor.lock())
			{
				float x = 300.0f + i * 350.0f;
				float y = 700.0f + (i % 2) * 100.0f;
				shape->SetAsRectangle({ 100.0f, 100.0f }, color(100 + i * 30, 150, 200));
				shape->SetPosition({ x, y });
			}
		}

		// =============================================================================
		// PHYSICS TEST - Spawn dynamic and static bodies
		// =============================================================================
		LOG("[PHYSICS TEST] Setting up physics test scene in PreConstruct...");

		// Create ground platform (static - doesn't move)
		auto GroundActor = SpawnActor<Actor>();
		if (auto ground = GroundActor.lock())
		{
			ground->SetAsRectangle({ 800.0f, 50.0f }, color(80, 80, 80));  // Wide grey platform
			ground->SetPosition({ 960.0f, 900.0f });  // Near bottom of screen
			ground->SetPhysicsType(PhysicsType::Static);  // Static body
			LOG("[PHYSICS TEST] Created static ground platform");
		}

		// Create falling box (dynamic - affected by gravity)
		auto FallingBox = SpawnActor<Actor>();
		if (auto box = FallingBox.lock())
		{
			box->SetAsRectangle({ 80.0f, 80.0f }, color(220, 100, 100));  // Red box
			box->SetPosition({ 960.0f, 200.0f });  // Start high above ground
			box->SetPhysicsType(PhysicsType::Dynamic);  // Dynamic body - will fall!
			PhysicsBox = box;
			LOG("[PHYSICS TEST] Created dynamic falling box at (960, 200)");
		}

		// Create a second falling box offset to the side
		auto FallingBox2 = SpawnActor<Actor>();
		if (auto box2 = FallingBox2.lock())
		{
			box2->SetAsRectangle({ 60.0f, 60.0f }, color(100, 220, 100));  // Green box
			box2->SetPosition({ 760.0f, 100.0f });  // Higher and to the left
			box2->SetPhysicsType(PhysicsType::Dynamic);
			LOG("[PHYSICS TEST] Created second dynamic box at (760, 100)");
		}
	}

	void MainMenu::BeginPlay()
	{
		// Music and ambient play automatically when game starts
		Subsystem.Audio->PlayMusic("Assets/Audio/Default/defaultMusic.ogg");
		Subsystem.Audio->PlayAmbient("Assets/Audio/Default/defaultAmbient.ogg");

		// =============================================================================
		// INPUT SYSTEM TESTS - REMOVE AFTER TESTING
		// =============================================================================
		SetupInputTests();
	}

	void MainMenu::EndPlay()
	{
		// Stop all audio when exiting Play mode
		Subsystem.Audio->StopAll();

		// =============================================================================
		// INPUT SYSTEM TESTS - REMOVE AFTER TESTING
		// =============================================================================
		CleanupInputTests();
	}

	void MainMenu::Tick(float DeltaTime)
	{
		// Simple animation - bob the title card
		if (auto title = TitleCard.lock())
		{
			vec2f Pos = title->GetActorPosition();
			static float Time = 0.0f;
			Time += DeltaTime;
			Pos.y = 300.0f + std::sin(Time * 2.0f) * 20.0f;
			title->SetPosition(Pos);
		}

		// =============================================================================
		// INPUT SYSTEM TESTS - STATE-BASED QUERY TESTING
		// REMOVE AFTER TESTING
		// =============================================================================
		if (Subsystem.Input)
		{
			// Reset movement
			TestMovementX = 0.0f;
			TestMovementY = 0.0f;

			// Check WASD movement (state-based IsPressed queries)
			if (Subsystem.Input->IsPressed(TEST_MOVE_UP))
			{
				TestMovementY -= 1.0f;
				LOG("[INPUT TEST] MOVE_UP is pressed (W key)");
			}
			if (Subsystem.Input->IsPressed(TEST_MOVE_DOWN))
			{
				TestMovementY += 1.0f;
				LOG("[INPUT TEST] MOVE_DOWN is pressed (S key)");
			}
			if (Subsystem.Input->IsPressed(TEST_MOVE_LEFT))
			{
				TestMovementX -= 1.0f;
				LOG("[INPUT TEST] MOVE_LEFT is pressed (A key)");
			}
			if (Subsystem.Input->IsPressed(TEST_MOVE_RIGHT))
			{
				TestMovementX += 1.0f;
				LOG("[INPUT TEST] MOVE_RIGHT is pressed (D key)");
			}

			// Test IsJustPressed - fires once per press
			if (Subsystem.Input->IsJustPressed(TEST_ACTION_JUMP))
			{
				LOG("[INPUT TEST] JUMP triggered (Space just pressed)");
				bTestJumpTriggered = true;
			}

			// Test gamepad buttons via IsPressed
			if (Subsystem.Input->IsPressed(TEST_GAMEPAD_SOUTH))
			{
				LOG("[INPUT TEST] Gamepad SOUTH button held (A on Xbox)");
			}
		}
		// END INPUT TESTS
	}

	// =============================================================================
	// INPUT SYSTEM TESTS - IMPLEMENTATION
	// REMOVE ALL OF THIS AFTER TESTING
	// =============================================================================
	void MainMenu::SetupInputTests()
	{
		if (!Subsystem.Input)
		{
			LOG("[INPUT TEST] Input subsystem is null!");
			return;
		}

		LOG("[INPUT TEST] ============================================");
		LOG("[INPUT TEST] Setting up input bindings...");
		LOG("[INPUT TEST] ============================================");

		// TEST 1: STATE-BASED INPUT (IsPressed queries in Tick)
		// Movement: WASD keys
		Subsystem.Input->Bind(TEST_MOVE_UP, Input::Keyboard{sf::Keyboard::Scan::W});
		Subsystem.Input->Bind(TEST_MOVE_DOWN, Input::Keyboard{sf::Keyboard::Scan::S});
		Subsystem.Input->Bind(TEST_MOVE_LEFT, Input::Keyboard{sf::Keyboard::Scan::A});
		Subsystem.Input->Bind(TEST_MOVE_RIGHT, Input::Keyboard{sf::Keyboard::Scan::D});
		LOG("[INPUT TEST] Bound WASD keys for movement (state-based)");

		// Alternative arrow key bindings (multiple bindings per action)
		Subsystem.Input->Bind(TEST_MOVE_UP, Input::Keyboard{sf::Keyboard::Scan::Up});
		Subsystem.Input->Bind(TEST_MOVE_DOWN, Input::Keyboard{sf::Keyboard::Scan::Down});
		Subsystem.Input->Bind(TEST_MOVE_LEFT, Input::Keyboard{sf::Keyboard::Scan::Left});
		Subsystem.Input->Bind(TEST_MOVE_RIGHT, Input::Keyboard{sf::Keyboard::Scan::Right});
		LOG("[INPUT TEST] Bound Arrow keys as alternative (multiple bindings)");

		// TEST 2: EVENT-BASED INPUT (OnPressed callback)
		// Jump: Spacebar - fires once per press
		Subsystem.Input->Bind(TEST_ACTION_JUMP, Input::Keyboard{sf::Keyboard::Scan::Space});
		auto jumpCallback = [this]()
		{
			LOG("[INPUT TEST] OnPressed CALLBACK: Jump! (Spacebar)");
		};
		Subsystem.Input->OnPressed(TEST_ACTION_JUMP, jumpCallback);
		LOG("[INPUT TEST] Bound Spacebar with OnPressed callback for JUMP");

		// TEST 3: EVENT-BASED INPUT (OnHeld callback)
		// Fire: Left Mouse - fires continuously while held
		Subsystem.Input->Bind(TEST_ACTION_FIRE, Input::Mouse{sf::Mouse::Button::Left});
		auto fireHeldCallback = [this]()
		{
			static int heldCounter = 0;
			// Fire immediately on first call (counter is 0), then every 60 frames
			if (heldCounter++ % 60 == 0)
			{
				LOG("[INPUT TEST] OnHeld CALLBACK: Firing... (LMB held)");
			}
		};
		Subsystem.Input->OnHeld(TEST_ACTION_FIRE, fireHeldCallback);
		LOG("[INPUT TEST] Bound Left Mouse with OnHeld callback for FIRE");

		// TEST 4: EVENT-BASED INPUT (OnReleased callback)
		// Interact: E key - fires when key released
		Subsystem.Input->Bind(TEST_ACTION_INTERACT, Input::Keyboard{sf::Keyboard::Scan::E});
		auto interactReleasedCallback = [this]()
		{
			LOG("[INPUT TEST] OnReleased CALLBACK: Interact released! (E key)");
		};
		Subsystem.Input->OnReleased(TEST_ACTION_INTERACT, interactReleasedCallback);
		LOG("[INPUT TEST] Bound E key with OnReleased callback for INTERACT");

		// TEST 5: GAMEPAD INPUT - South button (A on Xbox)
		Subsystem.Input->Bind(TEST_GAMEPAD_SOUTH, Input::Gamepad{GamepadButton::South, 0});
		auto gamepadSouthCallback = [this]()
		{
			LOG("[INPUT TEST] OnPressed CALLBACK: Gamepad SOUTH button pressed! (Player 1)");
		};
		Subsystem.Input->OnPressed(TEST_GAMEPAD_SOUTH, gamepadSouthCallback);
		LOG("[INPUT TEST] Bound Gamepad SOUTH button (A on Xbox)");

		// TEST 6: GAMEPAD INPUT - East button (B on Xbox)
		Subsystem.Input->Bind(TEST_GAMEPAD_EAST, Input::Gamepad{GamepadButton::East, 0});
		auto gamepadEastCallback = [this]()
		{
			LOG("[INPUT TEST] OnPressed CALLBACK: Gamepad EAST button pressed! (B on Xbox)");
		};
		Subsystem.Input->OnPressed(TEST_GAMEPAD_EAST, gamepadEastCallback);
		LOG("[INPUT TEST] Bound Gamepad EAST button (B on Xbox)");

		// TEST 7: GAMEPAD START BUTTON
		Subsystem.Input->Bind(TEST_GAMEPAD_START, Input::Gamepad{GamepadButton::Start, 0});
		auto gamepadStartCallback = [this]()
		{
			LOG("[INPUT TEST] OnPressed CALLBACK: Gamepad START button pressed!");
		};
		Subsystem.Input->OnPressed(TEST_GAMEPAD_START, gamepadStartCallback);
		LOG("[INPUT TEST] Bound Gamepad START button");

		// TEST 8: GUI NAVIGATION (Controller buttons)
		Subsystem.Input->Bind(TEST_GUI_CONFIRM, Input::Gamepad{GamepadButton::South, 0});
		Subsystem.Input->Bind(TEST_GUI_CANCEL, Input::Gamepad{GamepadButton::East, 0});
		LOG("[INPUT TEST] Bound GUI navigation buttons");

		// TEST 9: JOYSTICK AXIS (Analog stick as digital input)
		// Left Stick X axis -> MoveLeft (negative) / MoveRight (positive)
		Subsystem.Input->Bind(TEST_MOVE_LEFT, Input::JoystickAxis{sf::Joystick::Axis::X, -0.3f, 0});
		Subsystem.Input->Bind(TEST_MOVE_RIGHT, Input::JoystickAxis{sf::Joystick::Axis::X, 0.3f, 0});
		// Left Stick Y axis -> MoveUp (negative) / MoveDown (positive)
		Subsystem.Input->Bind(TEST_MOVE_UP, Input::JoystickAxis{sf::Joystick::Axis::Y, -0.3f, 0});
		Subsystem.Input->Bind(TEST_MOVE_DOWN, Input::JoystickAxis{sf::Joystick::Axis::Y, 0.3f, 0});
		LOG("[INPUT TEST] Bound Left Stick axes for movement (threshold +/-0.3)");

		LOG("[INPUT TEST] ============================================");
		LOG("[INPUT TEST] All input bindings complete!");
		LOG("[INPUT TEST] ============================================");
		LOG("[INPUT TEST] Keyboard: WASD/Arrows (move), Space (jump), E (interact)");
		LOG("[INPUT TEST] Mouse: Left Click (fire)");
		LOG("[INPUT TEST] Gamepad: South (A), East (B), Start buttons");
		LOG("[INPUT TEST] Joystick: Left Stick (movement, threshold +/-0.3)");
		LOG("[INPUT TEST] ============================================");
	}

	void MainMenu::CleanupInputTests()
	{
		if (!Subsystem.Input)
		{
			return;
		}

		LOG("[INPUT TEST] Cleaning up input bindings...");
		Subsystem.Input->UnbindAll();
		LOG("[INPUT TEST] Input bindings cleared");
	}
	// =============================================================================
	// END INPUT SYSTEM TESTS
	// =============================================================================
}

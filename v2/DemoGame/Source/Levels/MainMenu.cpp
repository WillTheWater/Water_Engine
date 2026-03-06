// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"

namespace we
{
    MainMenu::MainMenu(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    void MainMenu::BeginPlay()
    {
        LOG("MainMenu::BeginPlay")
        TestActor = SpawnActor<Actor>().lock();
        DestroyTimerHandle = GetTimer().SetTimer(GetObject(), &MainMenu::DestroyTestActor, 1.0f, false);
       
        auto Tex = LoadAsset().LoadTexture("Assets/Textures/Game/idle.png");
        TestActor->SetSprite(Tex);

		SetupInputTests();
    }

    void MainMenu::Tick(float DeltaTime)
    {
		TestMovementX = 0.0f;
		TestMovementY = 0.0f;

		if (InputController().IsPressed(TEST_MOVE_UP)) TestMovementY -= 1.0f;
		if (InputController().IsPressed(TEST_MOVE_DOWN)) TestMovementY += 1.0f;
		if (InputController().IsPressed(TEST_MOVE_LEFT)) TestMovementX -= 1.0f;
		if (InputController().IsPressed(TEST_MOVE_RIGHT)) TestMovementX += 1.0f;

		if (InputController().IsJustPressed(TEST_ACTION_JUMP))
		{
			bTestJumpTriggered = true;
		}
    }

    void MainMenu::DestroyTestActor()
    {
        LOG("Destroying TestActor!")
        if (TestActor)
        {
            TestActor->Destroy();
            TestActor = nullptr;
        }
    }

	void MainMenu::SetupInputTests()
	{
		// Movement: WASD keys
		InputController().Bind(TEST_MOVE_UP, Input::Keyboard{sf::Keyboard::Scan::W});
		InputController().Bind(TEST_MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::S });
		InputController().Bind(TEST_MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::A });
		InputController().Bind(TEST_MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::D });

		// Alternative arrow key bindings (multiple bindings per action)
		InputController().Bind(TEST_MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::Up });
		InputController().Bind(TEST_MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::Down });
		InputController().Bind(TEST_MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::Left });
		InputController().Bind(TEST_MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::Right });

		// Jump: Spacebar - fires once per press
		InputController().Bind(TEST_ACTION_JUMP, Input::Keyboard{ sf::Keyboard::Scan::Space });
		auto jumpCallback = [this]()
			{
				LOG("[INPUT TEST] OnPressed CALLBACK: Jump! (Spacebar)");
			};
		InputController().OnPressed(TEST_ACTION_JUMP, jumpCallback);

		// Fire: Left Mouse - fires continuously while held
		InputController().Bind(TEST_ACTION_FIRE, Input::Mouse{ sf::Mouse::Button::Left });
		auto fireHeldCallback = [this]()
			{
				static int heldCounter = 0;
				if (heldCounter++ % 60 == 0)
				{
					LOG("[INPUT TEST] OnHeld CALLBACK: Firing... (LMB held)");
				}
			};
		InputController().OnHeld(TEST_ACTION_FIRE, fireHeldCallback);

		// Interact: E key - fires when key released
		InputController().Bind(TEST_ACTION_INTERACT, Input::Keyboard{ sf::Keyboard::Scan::E });
		auto interactReleasedCallback = [this]()
			{
				LOG("[INPUT TEST] OnReleased CALLBACK: Interact released! (E key)");
			};
		InputController().OnReleased(TEST_ACTION_INTERACT, interactReleasedCallback);

		// GAMEPAD INPUT - South button (A on Xbox)
		InputController().Bind(TEST_GAMEPAD_SOUTH, Input::Gamepad{ GamepadButton::South, 0 });
		auto gamepadSouthCallback = [this]()
			{
				LOG("[INPUT TEST] OnPressed CALLBACK: Gamepad SOUTH button pressed! (Player 1)");
			};
		InputController().OnPressed(TEST_GAMEPAD_SOUTH, gamepadSouthCallback);

		// GAMEPAD INPUT - East button (B on Xbox)
		InputController().Bind(TEST_GAMEPAD_EAST, Input::Gamepad{ GamepadButton::East, 0 });
		auto gamepadEastCallback = [this]()
			{
				LOG("[INPUT TEST] OnPressed CALLBACK: Gamepad EAST button pressed! (B on Xbox)");
			};
		InputController().OnPressed(TEST_GAMEPAD_EAST, gamepadEastCallback);

		// GAMEPAD START BUTTON
		InputController().Bind(TEST_GAMEPAD_START, Input::Gamepad{ GamepadButton::Start, 0 });
		auto gamepadStartCallback = [this]()
			{
				LOG("[INPUT TEST] OnPressed CALLBACK: Gamepad START button pressed!");
			};
		InputController().OnPressed(TEST_GAMEPAD_START, gamepadStartCallback);

		// Left Stick X axis -> MoveLeft (negative) / MoveRight (positive)
		InputController().Bind(TEST_MOVE_LEFT, Input::JoystickAxis{ sf::Joystick::Axis::X, -0.3f, 0 });
		InputController().Bind(TEST_MOVE_RIGHT, Input::JoystickAxis{ sf::Joystick::Axis::X, 0.3f, 0 });
		// Left Stick Y axis -> MoveUp (negative) / MoveDown (positive)
		InputController().Bind(TEST_MOVE_UP, Input::JoystickAxis{ sf::Joystick::Axis::Y, -0.3f, 0 });
		InputController().Bind(TEST_MOVE_DOWN, Input::JoystickAxis{ sf::Joystick::Axis::Y, 0.3f, 0 });
	}
}
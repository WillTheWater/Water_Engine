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
#include "Tests/TGUITest.h"
#include "Tests/WidgetTests.h"

namespace we
{
    MainMenu::MainMenu(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    void MainMenu::BeginPlay()
    {
        TestActor = SpawnActor<Actor>().lock();
        DestroyTimerHandle = GetTimer().SetTimer(GetObject(), &MainMenu::DestroyTestActor, 1.0f, false);
       
        auto Tex = LoadAsset().LoadTexture("Assets/Textures/Game/idle.png");
        TestActor->SetSprite(Tex);

		SetupInputTests();

		// Widget tests
		WidgetTestSetup TestSetup;
		TestSetup.CreateScreenUI();
		TestSetup.CreateWorldUI();
    }

    void MainMenu::Tick(float DeltaTime)
    {
        // Test Movement (WASD)
        TestMovementX = 0.0f;
        TestMovementY = 0.0f;
        if (InputController().Pressed(MOVE_UP)) TestMovementY -= 1.0f;
        if (InputController().Pressed(MOVE_DOWN)) TestMovementY += 1.0f;
        if (InputController().Pressed(MOVE_LEFT)) TestMovementX -= 1.0f;
        if (InputController().Pressed(MOVE_RIGHT)) TestMovementX += 1.0f;
        
        if (TestMovementX != 0.0f || TestMovementY != 0.0f)
        {
            LOG("[INPUT TEST] Movement: X={:.1f}, Y={:.1f}", TestMovementX, TestMovementY);
        }
        
        // Test Jump (Spacebar) - One shot
        if (InputController().Pressed(ACTION_JUMP))
        {
            LOG("[INPUT TEST] Jump pressed!");
        }
        
        // Test Fire (Left Mouse) - Held/Repeat
        if (InputController().Pressed(ACTION_FIRE))
        {
            bool bJustStarted = (FireTimer == 0.0f);
            FireTimer += DeltaTime;
            
            if (bJustStarted || FireTimer >= FireInterval)
            {
                if (FireTimer >= FireInterval)
                    FireTimer = 0.0f;
                LOG("[INPUT TEST] FIRE!");
            }
        }
        else
        {
            FireTimer = 0.0f;
        }
        
        // Test Gamepad buttons
        if (InputController().Pressed(GAMEPAD_SOUTH))
        {
            LOG("[INPUT TEST] Gamepad SOUTH pressed!");
        }
        if (InputController().Pressed(GAMEPAD_EAST))
        {
            LOG("[INPUT TEST] Gamepad EAST pressed!");
        }
        if (InputController().Pressed(GAMEPAD_START))
        {
            LOG("[INPUT TEST] Gamepad START pressed!");
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
		auto& Input = InputController();
		
		// Keyboard movement
		Input.Bind(MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::W });
		Input.Bind(MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::S });
		Input.Bind(MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::A });
		Input.Bind(MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::D });
		
		// Actions
		Input.Bind(ACTION_JUMP, Input::Keyboard{ sf::Keyboard::Scan::Space });
		Input.Bind(ACTION_FIRE, Input::Mouse{ sf::Mouse::Button::Left });
		Input.Bind(ACTION_INTERACT, Input::Keyboard{ sf::Keyboard::Scan::E });
		
		// Gamepad
		Input.Bind(GAMEPAD_SOUTH, Input::Gamepad{ GamepadButton::South, 0 });
		Input.Bind(GAMEPAD_EAST, Input::Gamepad{ GamepadButton::East, 0 });
		Input.Bind(GAMEPAD_START, Input::Gamepad{ GamepadButton::Start, 0 });
		
		LOG("[MainMenu] Input tests bound!");
	}
}

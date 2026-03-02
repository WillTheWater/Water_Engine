// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Framework/EngineSubsystem.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"

namespace we
{
	MainMenu::MainMenu(EngineSubsystem& Subsystem)
		: World(Subsystem)
	{
	}

	void MainMenu::PreConstruct()
	{
		LOG("[MAINMENU] PreConstruct - Creating test scene");

		// Background
		auto BgActor = SpawnActor<Actor>();
		if (auto bg = BgActor.lock())
		{
			bg->SetAsRectangle({ 1920.0f, 1080.0f }, color(20, 30, 50));
			bg->SetPosition({ 960.0f, 540.0f });
			bg->SetCustomRenderDepth(-1000.0f);
			Background = bg;
		}

		// Title card
		auto TitleActor = SpawnActor<Actor>();
		if (auto title = TitleActor.lock())
		{
			title->SetAsRectangle({ 400.0f, 150.0f }, color(200, 180, 100));
			title->SetPosition({ 960.0f, 300.0f });
			TitleCard = title;
		}

		// Ground platform (static)
		auto Ground = SpawnActor<Actor>();
		if (auto ground = Ground.lock())
		{
			ground->SetAsRectangle({ 800.0f, 50.0f }, color(80, 80, 80));
			ground->SetPosition({ 960.0f, 900.0f });
			
			auto* proxy = ground->GetOrCreatePhysicsProxy();
			if (proxy)
			{
				proxy->SetSimulationType(PhysicsType::Static);
				proxy->SetCollisionChannel(CollisionChannel::WorldStatic);
				float Scale = ground->GetPhysicsScale();
				proxy->AddBox({ 400.0f * Scale, 25.0f * Scale });
				proxy->SetCollisionMask(CollisionPresets::WorldStatic_Mask);
			}
			GroundPlatform = ground;
		}

		// Dynamic box
		auto Box = SpawnActor<Actor>();
		if (auto box = Box.lock())
		{
			box->SetAsRectangle({ 80.0f, 80.0f }, color(220, 100, 100));
			box->SetPosition({ 960.0f, 200.0f });
			
			auto* proxy = box->GetOrCreatePhysicsProxy();
			if (proxy)
			{
				proxy->SetSimulationType(PhysicsType::Dynamic);
				proxy->SetCollisionChannel(CollisionChannel::PhysicsBody);
				float Scale = box->GetPhysicsScale();
				proxy->AddBox({ 40.0f * Scale, 40.0f * Scale });
				uint mask = CollisionPresets::Pawn_Mask | static_cast<uint>(CollisionChannel::Sensor);
				proxy->SetCollisionMask(mask);
				
				proxy->SetLinearDamping(8.0f);
				proxy->SetAngularDamping(5.0f);
				proxy->SetFixedRotation(true);
			}
			DynamicBox = box;
		}

		// Sensor zone
		auto Sensor = SpawnActor<Actor>();
		if (auto sensor = Sensor.lock())
		{
			sensor->SetAsCircle(100.0f, color(100, 200, 100, 100));
			sensor->SetPosition({ 960.0f, 600.0f });
			
			auto* proxy = sensor->GetOrCreatePhysicsProxy();
			if (proxy)
			{
				proxy->SetSimulationType(PhysicsType::Static);
				proxy->SetCollisionChannel(CollisionChannel::Sensor);
				float Scale = sensor->GetPhysicsScale();
				proxy->AddCircle(100.0f * Scale);
				proxy->SetSensor(true);
				proxy->SetCollisionMask(CollisionPresets::Sensor_Mask);
			}
			SensorZone = sensor;
		}
	}

	void MainMenu::BeginPlay()
	{
		
		Subsystem.Audio->PlayMusic("Assets/Audio/Default/defaultMusic.ogg");
		Subsystem.Audio->PlayAmbient("Assets/Audio/Default/defaultAmbient.ogg");

		SetupInputTests();
		
		// Set up 2-second looped timer test on DynamicBox
		TestTimer = TimerManager::Get().SetTimer(GetObject(), &MainMenu::TimerTestCallback, 2.0f, true);
	}

	void MainMenu::EndPlay()
	{
		Subsystem.Audio->StopAll();
		CleanupInputTests();
	}

	void MainMenu::Tick(float DeltaTime)
	{
		// Title card
		if (auto title = TitleCard.lock())
		{
			vec2f Pos = title->GetActorPosition();
			static float Time = 0.0f;
			Time += DeltaTime;
			Pos.y = 300.0f + std::sin(Time * 2.0f) * 20.0f;
			title->SetPosition(Pos);
		}

		// Input handling
		if (Subsystem.Input)
		{
			TestMovementX = 0.0f;
			TestMovementY = 0.0f;

			if (Subsystem.Input->IsPressed(TEST_MOVE_UP)) TestMovementY -= 1.0f;
			if (Subsystem.Input->IsPressed(TEST_MOVE_DOWN)) TestMovementY += 1.0f;
			if (Subsystem.Input->IsPressed(TEST_MOVE_LEFT)) TestMovementX -= 1.0f;
			if (Subsystem.Input->IsPressed(TEST_MOVE_RIGHT)) TestMovementX += 1.0f;

			if (Subsystem.Input->IsJustPressed(TEST_ACTION_JUMP))
			{
				bTestJumpTriggered = true;
			}
		}

		// Apply forces to dynamic box
		if (auto box = DynamicBox.lock())
		{
			if (auto* proxy = box->GetPhysicsProxy())
			{
				if (TestMovementX != 0.0f || TestMovementY != 0.0f)
				{
					const float DesiredSpeed = 400.0f;
					const float Damping = 8.0f;
					float PhysicsScale = box->GetPhysicsScale();
					float ForceMagnitude = Damping * (DesiredSpeed * PhysicsScale);
					
					vec2f Force(TestMovementX * ForceMagnitude, TestMovementY * ForceMagnitude);
					proxy->AddForce(Force);
				}

				if (bTestJumpTriggered)
				{
					bTestJumpTriggered = false;
					
					float DashSpeed = 300.0f * box->GetPhysicsScale();
					float Mass = proxy->GetMass();
					float ImpulseMagnitude = Mass * DashSpeed;
					
					proxy->AddImpulse({0.0f, -ImpulseMagnitude});
				}
			}
		}

		// Raycast test (press E)
		if (Subsystem.Input && Subsystem.Input->IsJustPressed(TEST_ACTION_INTERACT))
		{
			float PhysicsScale = Subsystem.Physics->GetPhysicsScale();
			vec2f Start = { 960.0f * PhysicsScale, 100.0f * PhysicsScale };
			vec2f End = { 960.0f * PhysicsScale, 1000.0f * PhysicsScale };
			vec2f Hit, Normal;
			
			if (Subsystem.Physics)
			{
				auto* HitProxy = Subsystem.Physics->Raycast(Start, End, Hit, Normal);
				if (HitProxy && HitProxy->GetOwner())
				{
					LOG("[PHYSICS] Raycast hit '{}' at [{:.2f}, {:.2f}] (meters)", 
					    typeid(*HitProxy->GetOwner()).name(), Hit.x, Hit.y);
				}
				else if (HitProxy)
				{
					LOG("[PHYSICS] Raycast hit unknown at [{:.2f}, {:.2f}] (meters)", Hit.x, Hit.y);
				}
				else
				{
					LOG("[PHYSICS] Raycast missed");
				}
			}
		}
	}

	void MainMenu::SetupInputTests()
	{
		// Movement: WASD keys
		Subsystem.Input->Bind(TEST_MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::W });
		Subsystem.Input->Bind(TEST_MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::S });
		Subsystem.Input->Bind(TEST_MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::A });
		Subsystem.Input->Bind(TEST_MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::D });

		// Alternative arrow key bindings (multiple bindings per action)
		Subsystem.Input->Bind(TEST_MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::Up });
		Subsystem.Input->Bind(TEST_MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::Down });
		Subsystem.Input->Bind(TEST_MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::Left });
		Subsystem.Input->Bind(TEST_MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::Right });

		// Jump: Spacebar - fires once per press
		Subsystem.Input->Bind(TEST_ACTION_JUMP, Input::Keyboard{ sf::Keyboard::Scan::Space });
		auto jumpCallback = [this]()
			{
				LOG("[INPUT TEST] OnPressed CALLBACK: Jump! (Spacebar)");
			};
		Subsystem.Input->OnPressed(TEST_ACTION_JUMP, jumpCallback);

		// Fire: Left Mouse - fires continuously while held
		Subsystem.Input->Bind(TEST_ACTION_FIRE, Input::Mouse{ sf::Mouse::Button::Left });
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

		// Interact: E key - fires when key released
		Subsystem.Input->Bind(TEST_ACTION_INTERACT, Input::Keyboard{ sf::Keyboard::Scan::E });
		auto interactReleasedCallback = [this]()
			{
				LOG("[INPUT TEST] OnReleased CALLBACK: Interact released! (E key)");
			};
		Subsystem.Input->OnReleased(TEST_ACTION_INTERACT, interactReleasedCallback);

		// GAMEPAD INPUT - South button (A on Xbox)
		Subsystem.Input->Bind(TEST_GAMEPAD_SOUTH, Input::Gamepad{ GamepadButton::South, 0 });
		auto gamepadSouthCallback = [this]()
			{
				LOG("[INPUT TEST] OnPressed CALLBACK: Gamepad SOUTH button pressed! (Player 1)");
			};
		Subsystem.Input->OnPressed(TEST_GAMEPAD_SOUTH, gamepadSouthCallback);

		// GAMEPAD INPUT - East button (B on Xbox)
		Subsystem.Input->Bind(TEST_GAMEPAD_EAST, Input::Gamepad{ GamepadButton::East, 0 });
		auto gamepadEastCallback = [this]()
			{
				LOG("[INPUT TEST] OnPressed CALLBACK: Gamepad EAST button pressed! (B on Xbox)");
			};
		Subsystem.Input->OnPressed(TEST_GAMEPAD_EAST, gamepadEastCallback);

		// GAMEPAD START BUTTON
		Subsystem.Input->Bind(TEST_GAMEPAD_START, Input::Gamepad{ GamepadButton::Start, 0 });
		auto gamepadStartCallback = [this]()
			{
				LOG("[INPUT TEST] OnPressed CALLBACK: Gamepad START button pressed!");
			};
		Subsystem.Input->OnPressed(TEST_GAMEPAD_START, gamepadStartCallback);

		// Left Stick X axis -> MoveLeft (negative) / MoveRight (positive)
		Subsystem.Input->Bind(TEST_MOVE_LEFT, Input::JoystickAxis{ sf::Joystick::Axis::X, -0.3f, 0 });
		Subsystem.Input->Bind(TEST_MOVE_RIGHT, Input::JoystickAxis{ sf::Joystick::Axis::X, 0.3f, 0 });
		// Left Stick Y axis -> MoveUp (negative) / MoveDown (positive)
		Subsystem.Input->Bind(TEST_MOVE_UP, Input::JoystickAxis{ sf::Joystick::Axis::Y, -0.3f, 0 });
		Subsystem.Input->Bind(TEST_MOVE_DOWN, Input::JoystickAxis{ sf::Joystick::Axis::Y, 0.3f, 0 });
	}

	void MainMenu::CleanupInputTests()
	{
		if (Subsystem.Input)
			Subsystem.Input->UnbindAll();
	}

	void MainMenu::TimerTestCallback()
	{
		LOG("Working Timer!");
	}
}

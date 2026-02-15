//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#include "Character/MovementComponent.h"
//#include "Framework/World/Actor/Actor.h"
//#include "Framework/EngineSubsystem.h"
//#include "Framework/World/World.h"
//#include "Subsystem/InputSubsystem.h"
//#include "Utility/Log.h"
//
//namespace we
//{
//	MovementComponent::MovementComponent(Actor* InOwner)
//		: Owner(InOwner)
//	{
//	}
//
//	void MovementComponent::BeginPlay()
//	{
//		BindInput();
//	}
//
//	void MovementComponent::Tick(float DeltaTime)
//	{
//		if (!Owner) return;
//
//		vec2f RawInput;
//
//		if (Owner->GetWorld()->GetSubsystem().Input->IsPressed(MOVE_UP))
//			RawInput.y -= 1;
//		if (Owner->GetWorld()->GetSubsystem().Input->IsPressed(MOVE_DOWN))
//			RawInput.y += 1;
//		if (Owner->GetWorld()->GetSubsystem().Input->IsPressed(MOVE_LEFT))
//			RawInput.x -= 1;
//		if (Owner->GetWorld()->GetSubsystem().Input->IsPressed(MOVE_RIGHT))
//			RawInput.x += 1;
//
//		float AxisX = Owner->GetWorld()->GetSubsystem().Input->GetAxisValue(0, sf::Joystick::Axis::X);
//		float AxisY = Owner->GetWorld()->GetSubsystem().Input->GetAxisValue(0, sf::Joystick::Axis::Y);
//
//		if (std::abs(AxisX) > EC.JoystickDeadzone)
//			RawInput.x += AxisX;
//		if (std::abs(AxisY) > EC.JoystickDeadzone)
//			RawInput.y += AxisY;
//
//		// Normalize movement
//		if (RawInput.lengthSquared() > 0)
//		{
//			InputVector = RawInput.normalized();
//			UpdateFacingDirection();
//		}
//		else
//		{
//			InputVector = {};
//		}
//
//		Velocity = InputVector * Speed;
//		Owner->SetVelocity(Velocity);
//	}
//
//	void MovementComponent::EndPlay()
//	{
//	}
//
//	Actor* MovementComponent::GetOwner() const
//	{
//		return Owner;
//	}
//
//	void MovementComponent::AddInputVector(const vec2f& Direction)
//	{
//		InputVector += Direction;
//	}
//
//	void MovementComponent::SetSpeed(float InSpeed)
//	{
//		Speed = InSpeed;
//	}
//
//	void MovementComponent::BindInput()
//	{
//		auto& Input = Owner->GetWorld()->GetSubsystem().Input;
//
//		// WASD
//		Input->Bind(MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::W });
//		Input->Bind(MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::S });
//		Input->Bind(MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::A });
//		Input->Bind(MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::D });
//
//		// Arrow keys
//		Input->Bind(MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::Up });
//		Input->Bind(MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::Down });
//		Input->Bind(MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::Left });
//		Input->Bind(MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::Right });
//
//		// Interact
//		Input->Bind(INTERACT, Input::Keyboard{ sf::Keyboard::Scan::E });
//		Input->Bind(INTERACT, Input::Gamepad{ GamepadButton::South, 0 });
//	}
//
//	void MovementComponent::UpdateFacingDirection()
//	{
//		if (InputVector.lengthSquared() < 0.01f) return;
//
//		// 8-way direction: calculate angle and snap to nearest 45 degrees
//		float Angle = std::atan2(InputVector.y, InputVector.x); // -PI to PI
//
//		// Convert to 8-direction index: 0=Right, 1=DownRight, 2=Down, 3=DownLeft, 4=Left, 5=UpLeft, 6=Up, 7=UpRight
//		// Angle 0 = Right, PI/2 = Down, PI = Left, -PI/2 = Up
//		int DirIndex = int(std::round(Angle / (PI / 4.0f))) % 8;
//		if (DirIndex < 0) DirIndex += 8;
//
//		// Set ForwardVector based on direction index
//		static const vec2f Directions[8] = {
//			{ 1, 0 },    // Right (0)
//			{ 1, 1 },    // DownRight (1) - normalized below
//			{ 0, 1 },    // Down (2)
//			{ -1, 1 },   // DownLeft (3)
//			{ -1, 0 },   // Left (4)
//			{ -1, -1 },  // UpLeft (5)
//			{ 0, -1 },   // Up (6)
//			{ 1, -1 }    // UpRight (7)
//		};
//
//		ForwardVector = Directions[DirIndex];
//		// Normalize diagonals
//		if (DirIndex % 2 == 1) {
//			ForwardVector = ForwardVector.normalized();
//		}
//
//		// Right vector is always 90 degrees clockwise from forward
//		RightVector = { -ForwardVector.y, ForwardVector.x };
//	}
//}
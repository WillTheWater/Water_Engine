// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/MovementComponent.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/World/World.h"
#include "Subsystem/InputSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	MovementComponent::MovementComponent(Actor* InOwner)
		: Owner(InOwner)
	{
	}

	void MovementComponent::BeginPlay()
	{
		BindInput();
	}

	void MovementComponent::Tick(float DeltaTime)
	{
		if (!Owner) return;

		vec2f RawInput;

		if (Owner->GetWorld()->GetSubsystem().Input->IsPressed(MOVE_UP))
			RawInput.y -= 1;
		if (Owner->GetWorld()->GetSubsystem().Input->IsPressed(MOVE_DOWN))
			RawInput.y += 1;
		if (Owner->GetWorld()->GetSubsystem().Input->IsPressed(MOVE_LEFT))
			RawInput.x -= 1;
		if (Owner->GetWorld()->GetSubsystem().Input->IsPressed(MOVE_RIGHT))
			RawInput.x += 1;

		float AxisX = Owner->GetWorld()->GetSubsystem().Input->GetAxisValue(0, sf::Joystick::Axis::X);
		float AxisY = Owner->GetWorld()->GetSubsystem().Input->GetAxisValue(0, sf::Joystick::Axis::Y);

		if (std::abs(AxisX) > EC.JoystickDeadzone)
			RawInput.x += AxisX;
		if (std::abs(AxisY) > EC.JoystickDeadzone)
			RawInput.y += AxisY;

		// Normalize movement
		if (RawInput.lengthSquared() > 0)
		{
			InputVector = RawInput.normalized();
			UpdateFacingDirection();
		}
		else
		{
			InputVector = {};
		}

		Velocity = InputVector * Speed;
		Owner->SetPosition(Owner->GetPosition() + Velocity * DeltaTime);
	}

	void MovementComponent::EndPlay()
	{
	}

	Actor* MovementComponent::GetOwner() const
	{
		return Owner;
	}

	void MovementComponent::AddInputVector(const vec2f& Direction)
	{
		InputVector += Direction;
	}

	void MovementComponent::SetSpeed(float InSpeed)
	{
		Speed = InSpeed;
	}

	void MovementComponent::BindInput()
	{
		auto& Input = Owner->GetWorld()->GetSubsystem().Input;

		// WASD
		Input->Bind(MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::W });
		Input->Bind(MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::S });
		Input->Bind(MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::A });
		Input->Bind(MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::D });

		// Arrow keys
		Input->Bind(MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::Up });
		Input->Bind(MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::Down });
		Input->Bind(MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::Left });
		Input->Bind(MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::Right });

		// Interact
		Input->Bind(INTERACT, Input::Keyboard{ sf::Keyboard::Scan::E });
		Input->Bind(INTERACT, Input::Gamepad{ GamepadButton::South, 0 });
	}

	void MovementComponent::UpdateFacingDirection()
	{
		if (InputVector.lengthSquared() < 0.01f) return;

		// Determine which direction
		float DotForward = InputVector.dot(ForwardVector);
		float DotRight = InputVector.dot(RightVector);

		// Threshold to prevent jitter
		const float Threshold = 0.5f;

		if (DotForward > Threshold)
		{
			// Moving forward (already facing this way)
			// Keep current ForwardVector
		}
		else if (DotForward < -Threshold)
		{
			// Moving backward - flip to opposite
			ForwardVector = -ForwardVector;
		}
		else if (DotRight > Threshold)
		{
			// Moving right relative to current facing
			// Rotate 90 degrees: (x,y) -> (-y,x) for right turn
			ForwardVector = { -ForwardVector.y, ForwardVector.x };
		}
		else if (DotRight < -Threshold)
		{
			// Moving left relative to current facing
			// Rotate -90 degrees: (x,y) -> (y,-x) for left turn
			ForwardVector = { ForwardVector.y, -ForwardVector.x };
		}
		RightVector = { -ForwardVector.y, ForwardVector.x };

		//LOG("Forward: ({:.0f}, {:.0f}) | Right: ({:.0f}, {:.0f})",
		//ForwardVector.x, ForwardVector.y, RightVector.x, RightVector.y);
	}
}
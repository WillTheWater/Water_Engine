// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/MovementComponent.h"
#include "Framework/World/Actor.h"
#include "Utility/Math.h"

namespace we
{
	MovementComponent::MovementComponent(Actor* InOwner)
		: Owner(InOwner)
	{
	}

	void MovementComponent::BeginPlay()
	{
	}

	void MovementComponent::Tick(float DeltaTime)
	{
		if (!Owner)
			return;

		Velocity = CalculateVelocity();
		
		if (bAutoOrient && bIsMoving)
		{
			UpdateOrientation();
		}

		OnVelocityCalculated.Broadcast(Velocity);
		
		InputVector = {};
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

	void MovementComponent::ClearInput()
	{
		InputVector = {};
	}

	vec2f MovementComponent::GetInputDirection() const
	{
		return LengthSquared(InputVector) > 0.001f ? Normalize(InputVector) : vec2f{};
	}

	void MovementComponent::SetFacingDirection(const vec2f& Direction)
	{
		if (LengthSquared(Direction) > 0.001f)
		{
			ForwardVector = Normalize(Direction);
			RightVector = Perpendicular(ForwardVector);
		}
	}

	void MovementComponent::SetLastMoveDirection(const vec2f& Dir)
	{
		if (LengthSquared(Dir) > 0.001f)
		{
			LastMoveDir = Normalize(Dir);
		}
	}

	vec2f MovementComponent::CalculateVelocity()
	{
		bIsMoving = LengthSquared(InputVector) > 0.001f;
		if (bIsMoving)
		{
			vec2f NormalizedInput = Normalize(InputVector);
			LastMoveDir = NormalizedInput;
			return NormalizedInput * CurrentSpeed;
		}
		return vec2f{};
	}

	vec2f MovementComponent::GetLocalInput() const
	{
		if (LengthSquared(InputVector) < 0.001f)
			return {};

		vec2f NormalizedInput = Normalize(InputVector);
		float ForwardDot = Dot(NormalizedInput, ForwardVector);
		float RightDot = Dot(NormalizedInput, RightVector);

		return { RightDot, ForwardDot };
	}

	void MovementComponent::UpdateOrientation()
	{
		if (LengthSquared(Velocity) > 0.001f)
		{
			ForwardVector = Normalize(Velocity);
			RightVector = Perpendicular(ForwardVector);
		}
	}

	void MovementComponent::ApplyMovement(float DeltaTime)
	{
		if (Owner && Velocity.lengthSquared() > 0.0f)
		{
			vec2f NewPosition = Owner->GetPosition() + (Velocity * DeltaTime);
			Owner->SetPosition(NewPosition);
		}
	}

	const drawable* MovementComponent::DrawDebug()
	{
		if (!Owner)
			return nullptr;

		if (!DebugArrow.has_value())
		{
			DebugArrow = sf::VertexArray(sf::PrimitiveType::Triangles, 9);
		}

		vec2f Pos = Owner->GetPosition();
		vec2f F = ForwardVector;
		vec2f R = RightVector;
		color Yellow = color::Yellow;

		float w = 2.f;     // half stem width
		float sl = 60.f;   // stem length
		float hw = 8.f;    // half head width
		float hl = 20.f;   // head length

		// Stem corners (relative to actor pos, extending along Forward)
		vec2f BackLeft  = Pos - R * w;
		vec2f BackRight = Pos + R * w;
		vec2f FrontLeft = Pos - R * w + F * sl;
		vec2f FrontRight= Pos + R * w + F * sl;

		// Arrow head points
		vec2f HeadLeft  = Pos - R * hw + F * sl;
		vec2f HeadRight = Pos + R * hw + F * sl;
		vec2f HeadTip   = Pos + F * (sl + hl);

		// Stem: first triangle (BackLeft, BackRight, FrontRight)
		(*DebugArrow)[0] = sf::Vertex(BackLeft, Yellow);
		(*DebugArrow)[1] = sf::Vertex(BackRight, Yellow);
		(*DebugArrow)[2] = sf::Vertex(FrontRight, Yellow);

		// Stem: second triangle (BackLeft, FrontRight, FrontLeft)
		(*DebugArrow)[3] = sf::Vertex(BackLeft, Yellow);
		(*DebugArrow)[4] = sf::Vertex(FrontRight, Yellow);
		(*DebugArrow)[5] = sf::Vertex(FrontLeft, Yellow);

		// Arrow head triangle (HeadLeft, HeadRight, HeadTip)
		(*DebugArrow)[6] = sf::Vertex(HeadLeft, Yellow);
		(*DebugArrow)[7] = sf::Vertex(HeadRight, Yellow);
		(*DebugArrow)[8] = sf::Vertex(HeadTip, Yellow);

		return &DebugArrow.value();
	}
}
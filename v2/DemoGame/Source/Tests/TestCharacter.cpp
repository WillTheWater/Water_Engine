// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Tests/TestCharacter.h"
#include "Component/AnimationComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"
#include <cmath>

namespace we
{
	TestCharacter::TestCharacter(World& OwningWorld)
		: Character(OwningWorld)
	{
	}

	TestCharacter::~TestCharacter() = default;

	void TestCharacter::BeginPlay()
	{
		PhysicsComp->SetBodyType(b2_dynamicBody);
		PhysicsComp->SetShapeType(PhysicsComponent::EShapeType::Circle);
		PhysicsComp->SetShapeSize({42.0f, 42.0f});
		PhysicsComp->SetLinearDamping(10.0f);

		CollComp->SetRadius(64.0f);
		MoveComp->SetSpeed(240);
		SetScale({ 2.1,2.1 });

		Character::BeginPlay();

		SetupAnimations();
		BindInput();
		
		// Configure camera
		if (auto CamComp = GetCameraComponent())
		{
			CamComp->AttachTo(this);
			CamComp->SetActive();
			CamComp->SetSmoothFollow(true, 0.1f);
			SetCameraBounds();
		}
	}

	void TestCharacter::SetupAnimations()
	{
		SpriteSheet IdleSheet("Assets/Textures/Game/idle.png", vec2u{100, 128}, 8);
		AnimComp->AddSpriteSheet(ETestCharSheet::Idle, IdleSheet);
		
		SpriteSheet WalkSheet("Assets/Textures/Game/walk.png", vec2u{100, 128}, 8);
		AnimComp->AddSpriteSheet(ETestCharSheet::Walk, WalkSheet);

		// 8-way idle animations
		AnimComp->AddAnimation(Animation{ETestCharAnim::IdleForward, ETestCharSheet::Idle, vec2u{0, 0}, vec2u{0, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::IdleForwardRight, ETestCharSheet::Idle, vec2u{1, 0}, vec2u{1, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::IdleRight, ETestCharSheet::Idle, vec2u{2, 0}, vec2u{2, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::IdleBackRight, ETestCharSheet::Idle, vec2u{3, 0}, vec2u{3, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::IdleBack, ETestCharSheet::Idle, vec2u{4, 0}, vec2u{4, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::IdleBackLeft, ETestCharSheet::Idle, vec2u{5, 0}, vec2u{5, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::IdleLeft, ETestCharSheet::Idle, vec2u{6, 0}, vec2u{6, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::IdleForwardLeft, ETestCharSheet::Idle, vec2u{7, 0}, vec2u{7, 7}});

		// 8-way walk animations
		AnimComp->AddAnimation(Animation{ETestCharAnim::WalkForward, ETestCharSheet::Walk, vec2u{0, 0}, vec2u{0, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::WalkForwardRight, ETestCharSheet::Walk, vec2u{1, 0}, vec2u{1, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::WalkRight, ETestCharSheet::Walk, vec2u{2, 0}, vec2u{2, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::WalkBackRight, ETestCharSheet::Walk, vec2u{3, 0}, vec2u{3, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::WalkBack, ETestCharSheet::Walk, vec2u{4, 0}, vec2u{4, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::WalkBackLeft, ETestCharSheet::Walk, vec2u{5, 0}, vec2u{5, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::WalkLeft, ETestCharSheet::Walk, vec2u{6, 0}, vec2u{6, 7}});
		AnimComp->AddAnimation(Animation{ETestCharAnim::WalkForwardLeft, ETestCharSheet::Walk, vec2u{7, 0}, vec2u{7, 7}});

		// Start with forward-facing idle
		AnimComp->TransitionTo(ETestCharAnim::IdleForward);
	}

	void TestCharacter::Tick(float DeltaTime)
	{
		HandleInput();
		UpdateDirectionalAnimation();
		
		Character::Tick(DeltaTime);
	}

	void TestCharacter::EndPlay()
	{
		Character::EndPlay();
	}

	void TestCharacter::BindInput()
	{
		auto& Input = InputController();
		
		Input.Bind(MOVE_UP, Input::Keyboard{sf::Keyboard::Scan::W});
		Input.Bind(MOVE_DOWN, Input::Keyboard{sf::Keyboard::Scan::S});
		Input.Bind(MOVE_LEFT, Input::Keyboard{sf::Keyboard::Scan::A});
		Input.Bind(MOVE_RIGHT, Input::Keyboard{sf::Keyboard::Scan::D});
		
		Input.Bind(MOVE_UP, Input::Keyboard{sf::Keyboard::Scan::Up});
		Input.Bind(MOVE_DOWN, Input::Keyboard{sf::Keyboard::Scan::Down});
		Input.Bind(MOVE_LEFT, Input::Keyboard{sf::Keyboard::Scan::Left});
		Input.Bind(MOVE_RIGHT, Input::Keyboard{sf::Keyboard::Scan::Right});
	}

	void TestCharacter::HandleInput()
	{
		if (!MoveComp) return;

		auto& Input = InputController();
		vec2f InputDir{};

		if (Input.Pressed(MOVE_UP)) InputDir.y -= 1.0f;
		if (Input.Pressed(MOVE_DOWN)) InputDir.y += 1.0f;
		if (Input.Pressed(MOVE_LEFT)) InputDir.x -= 1.0f;
		if (Input.Pressed(MOVE_RIGHT)) InputDir.x += 1.0f;

		if (InputDir.lengthSquared() > 0.0f)
		{
			MoveComp->AddInputVector(InputDir);
		}
	}

	void TestCharacter::UpdateDirectionalAnimation()
	{
		if (!AnimComp || !MoveComp) return;

		vec2f LastDir = MoveComp->GetLastMoveDirection();
		ETestCharAnim TargetAnim = MoveComp->IsMoving() 
			? DirectionToWalkAnim(LastDir) 
			: DirectionToIdleAnim(LastDir);
			
		if (!AnimComp->IsPlaying(static_cast<uint8>(TargetAnim)))
		{
			AnimComp->TransitionTo(TargetAnim);
		}
	}

	ETestCharAnim TestCharacter::DirectionToIdleAnim(const vec2f& Dir) const
	{
		vec2f N = Dir;
		float LenSq = N.lengthSquared();
		if (LenSq > 0.0f) N /= std::sqrt(LenSq);

		float Angle = std::atan2(N.y, N.x);
		float Degrees = Angle * 180.0f / 3.14159f;
		
		float Normalized = Degrees + 90.0f;
		if (Normalized < 0.0f) Normalized += 360.0f;
		if (Normalized >= 360.0f) Normalized -= 360.0f;
		
		int Sector = static_cast<int>((Normalized + 22.5f) / 45.0f) % 8;
		
		switch (Sector)
		{
			case 0: return ETestCharAnim::IdleBack;
			case 1: return ETestCharAnim::IdleBackRight;
			case 2: return ETestCharAnim::IdleRight;
			case 3: return ETestCharAnim::IdleForwardRight;
			case 4: return ETestCharAnim::IdleForward;
			case 5: return ETestCharAnim::IdleForwardLeft;
			case 6: return ETestCharAnim::IdleLeft;
			case 7: return ETestCharAnim::IdleBackLeft;
			default: return ETestCharAnim::IdleForward;
		}
	}

	ETestCharAnim TestCharacter::DirectionToWalkAnim(const vec2f& Dir) const
	{
		vec2f N = Dir;
		float LenSq = N.lengthSquared();
		if (LenSq > 0.0f) N /= std::sqrt(LenSq);

		float Angle = std::atan2(N.y, N.x);
		float Degrees = Angle * 180.0f / 3.14159f;
		
		float Normalized = Degrees + 90.0f;
		if (Normalized < 0.0f) Normalized += 360.0f;
		if (Normalized >= 360.0f) Normalized -= 360.0f;
		
		int Sector = static_cast<int>((Normalized + 22.5f) / 45.0f) % 8;
		
		switch (Sector)
		{
			case 0: return ETestCharAnim::WalkBack;
			case 1: return ETestCharAnim::WalkBackRight;
			case 2: return ETestCharAnim::WalkRight;
			case 3: return ETestCharAnim::WalkForwardRight;
			case 4: return ETestCharAnim::WalkForward;
			case 5: return ETestCharAnim::WalkForwardLeft;
			case 6: return ETestCharAnim::WalkLeft;
			case 7: return ETestCharAnim::WalkBackLeft;
			default: return ETestCharAnim::WalkForward;
		}
	}
	
	void TestCharacter::SetCameraBounds()
	{
		if (auto CamComp = GetCameraComponent())
		{
			// World bounds: (0,0) to (5760, 3240)
			constexpr vec2f WorldSize = {5760.0f, 3240.0f};
			
			// Viewport size (1920x1080)
			constexpr vec2f ViewSize = {1920.0f, 1080.0f};
			
			// Camera center clamping - viewport half-size from edges
			vec2f MinBounds = ViewSize / 2.0f;  // (960, 540)
			vec2f MaxBounds = WorldSize - MinBounds;  // (4800, 2700)
			
			CamComp->SetBounds({MinBounds, MaxBounds - MinBounds});
		}
	}
}

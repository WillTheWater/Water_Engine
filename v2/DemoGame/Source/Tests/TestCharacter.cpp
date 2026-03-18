// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Tests/TestCharacter.h"
#include "Component/AnimationComponent.h"
#include <cmath>
#include "Component/PhysicsComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/MovementComponent.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Input/InputActions.h"

namespace we
{
	TestCharacter::TestCharacter(World& OwningWorld)
		: Actor(OwningWorld)
	{
	}

	TestCharacter::~TestCharacter() = default;

	void TestCharacter::BeginPlay()
	{
		Actor::BeginPlay();

		AnimComp = make_shared<AnimationComponent>(this);
		SetupAnimations();
		AnimComp->BeginPlay();

		PhysicsComp = make_shared<PhysicsComponent>(this);
		PhysicsComp->BeginPlay();

		CollisionComp = make_shared<CollisionComponent>(this);
		CollisionComp->SetRadius(64.0f);
		CollisionComp->BeginPlay();
		CollisionComp->DrawDebug();

		MoveComp = make_shared<MovementComponent>(this);
		MoveComp->BeginPlay();

		BindInput();
	}

	void TestCharacter::SetupAnimations()
	{
		// Sprite sheets: 2048x2048, 8x8 grid, each frame 256x256
		SpriteSheet IdleSheet("Assets/Textures/Game/idle.png", vec2u{256, 256}, 8);
		AnimComp->AddSpriteSheet(ETestCharSheet::Idle, IdleSheet);
		
		SpriteSheet WalkSheet("Assets/Textures/Game/walk.png", vec2u{256, 256}, 8);
		AnimComp->AddSpriteSheet(ETestCharSheet::Walk, WalkSheet);

		// 8-way idle animations, 8 frames each
		// Row 0: Forward (Down/South)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::IdleForward,
			ETestCharSheet::Idle,
			vec2u{0, 0}, vec2u{0, 7}
		});

		// Row 1: Forward-Right (Southeast)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::IdleForwardRight,
			ETestCharSheet::Idle,
			vec2u{1, 0}, vec2u{1, 7}
		});

		// Row 2: Right (East)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::IdleRight,
			ETestCharSheet::Idle,
			vec2u{2, 0}, vec2u{2, 7}
		});

		// Row 3: Back-Right (Northeast)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::IdleBackRight,
			ETestCharSheet::Idle,
			vec2u{3, 0}, vec2u{3, 7}
		});

		// Row 4: Back (North)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::IdleBack,
			ETestCharSheet::Idle,
			vec2u{4, 0}, vec2u{4, 7}
		});

		// Row 5: Back-Left (Northwest)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::IdleBackLeft,
			ETestCharSheet::Idle,
			vec2u{5, 0}, vec2u{5, 7}
		});

		// Row 6: Left (West)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::IdleLeft,
			ETestCharSheet::Idle,
			vec2u{6, 0}, vec2u{6, 7}
		});

		// Row 7: Forward-Left (Southwest)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::IdleForwardLeft,
			ETestCharSheet::Idle,
			vec2u{7, 0}, vec2u{7, 7}
		});

		// 8-way walk animations, 8 frames each (same row layout)
		// Row 0: Forward (Down/South)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::WalkForward,
			ETestCharSheet::Walk,
			vec2u{0, 0}, vec2u{0, 7}
		});

		// Row 1: Forward-Right (Southeast)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::WalkForwardRight,
			ETestCharSheet::Walk,
			vec2u{1, 0}, vec2u{1, 7}
		});

		// Row 2: Right (East)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::WalkRight,
			ETestCharSheet::Walk,
			vec2u{2, 0}, vec2u{2, 7}
		});

		// Row 3: Back-Right (Northeast)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::WalkBackRight,
			ETestCharSheet::Walk,
			vec2u{3, 0}, vec2u{3, 7}
		});

		// Row 4: Back (North)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::WalkBack,
			ETestCharSheet::Walk,
			vec2u{4, 0}, vec2u{4, 7}
		});

		// Row 5: Back-Left (Northwest)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::WalkBackLeft,
			ETestCharSheet::Walk,
			vec2u{5, 0}, vec2u{5, 7}
		});

		// Row 6: Left (West)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::WalkLeft,
			ETestCharSheet::Walk,
			vec2u{6, 0}, vec2u{6, 7}
		});

		// Row 7: Forward-Left (Southwest)
		AnimComp->AddAnimation(Animation{
			ETestCharAnim::WalkForwardLeft,
			ETestCharSheet::Walk,
			vec2u{7, 0}, vec2u{7, 7}
		});

		// Start with forward-facing idle
		AnimComp->TransitionTo(ETestCharAnim::IdleForward);
	}

	void TestCharacter::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);

		// Handle input movement (separated for easy removal)
		HandleInput();
		
		// Update facing direction based on movement
		UpdateDirectionalAnimation();

		if (AnimComp)
		{
			AnimComp->Tick(DeltaTime);
		}

		if (PhysicsComp)
		{
			PhysicsComp->Tick(DeltaTime);
		}

		if (CollisionComp)
		{
			CollisionComp->Tick(DeltaTime);
		}

		if (MoveComp)
		{
			MoveComp->Tick(DeltaTime);
		}
	}

	void TestCharacter::EndPlay()
	{
		if (AnimComp)
		{
			AnimComp->EndPlay();
			AnimComp.reset();
		}

		if (PhysicsComp)
		{
			PhysicsComp->EndPlay();
			PhysicsComp.reset();
		}

		if (CollisionComp)
		{
			CollisionComp->EndPlay();
			CollisionComp.reset();
		}

		if (MoveComp)
		{
			MoveComp->EndPlay();
			MoveComp.reset();
		}

		Actor::EndPlay();
	}

	void TestCharacter::GetDrawables(vector<const drawable*>& OutDrawables) const
	{
		Actor::GetDrawables(OutDrawables);

		if (CollisionComp)
		{
			if (const auto* Debug = CollisionComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}

		if (MoveComp)
		{
			if (const auto* Debug = MoveComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}
	}

	void TestCharacter::BindInput()
	{
		auto& Input = InputController();

		// WASD
		Input.Bind(MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::W });
		Input.Bind(MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::S });
		Input.Bind(MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::A });
		Input.Bind(MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::D });

		// Arrow keys
		Input.Bind(MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::Up });
		Input.Bind(MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::Down });
		Input.Bind(MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::Left });
		Input.Bind(MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::Right });
	}

	void TestCharacter::HandleInput()
	{
		if (!MoveComp)
			return;

		auto& Input = InputController();
		vec2f InputDir{};

		if (Input.Pressed(MOVE_UP))
			InputDir.y -= 1.0f;
		if (Input.Pressed(MOVE_DOWN))
			InputDir.y += 1.0f;
		if (Input.Pressed(MOVE_LEFT))
			InputDir.x -= 1.0f;
		if (Input.Pressed(MOVE_RIGHT))
			InputDir.x += 1.0f;

		if (InputDir.lengthSquared() > 0.0f)
		{
			MoveComp->AddInputVector(InputDir);
		}
	}

	void TestCharacter::UpdateDirectionalAnimation()
	{
		if (!AnimComp || !MoveComp)
			return;

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
		// Normalize direction
		vec2f N = Dir;
		float LenSq = N.lengthSquared();
		if (LenSq > 0.0f)
		{
			N /= std::sqrt(LenSq);
		}

		float Angle = std::atan2(N.y, N.x);
		float Degrees = Angle * 180.0f / 3.14159f;
		float FacingDegrees = Degrees + 90.0f;
		
		// Convert to 0-360 range with 0 at Back, going clockwise
		float Normalized = Degrees + 90.0f;
		if (Normalized < 0.0f)
			Normalized += 360.0f;
		if (Normalized >= 360.0f)
			Normalized -= 360.0f;
		
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
		// Normalize direction
		vec2f N = Dir;
		float LenSq = N.lengthSquared();
		if (LenSq > 0.0f)
		{
			N /= std::sqrt(LenSq);
		}

		float Angle = std::atan2(N.y, N.x);
		float Degrees = Angle * 180.0f / 3.14159f;
		
		// Convert to 0-360 range with 0 at Back, going clockwise
		float Normalized = Degrees + 90.0f;
		if (Normalized < 0.0f)
			Normalized += 360.0f;
		if (Normalized >= 360.0f)
			Normalized -= 360.0f;
		
		// 8 sectors of 45 degrees, offset by 22.5 to center on directions
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
}
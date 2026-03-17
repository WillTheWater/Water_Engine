// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Tests/TestCharacter.h"
#include "Component/AnimationComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/CollisionComponent.h"
#include "Subsystem/ResourceSubsystem.h"

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
		CollisionComp->BeginPlay();
	}

	void TestCharacter::SetupAnimations()
	{
		// Sprite sheet: 2048x2048, 8x8 grid, each frame 256x256
		SpriteSheet IdleSheet("Assets/Textures/Game/idle.png", vec2u{256, 256}, 8);
		AnimComp->AddSpriteSheet(ETestCharSheet::Idle, IdleSheet);

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

		// Start with forward-facing idle
		AnimComp->TransitionTo(ETestCharAnim::IdleForward);
	}

	void TestCharacter::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);

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

		Actor::EndPlay();
	}
}
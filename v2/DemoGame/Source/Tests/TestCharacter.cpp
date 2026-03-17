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
	enum class EIdleDirection : uint8
	{
		Forward = 1,  
		ForwardRight, 
		Right,        
		BackRight,    
		Back,         
		BackLeft,     
		Left,         
		ForwardLeft   
	};

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
		AnimComp->AddSpriteSheet(1, IdleSheet);

		// 8-way idle animations, 8 frames each
		// Row 0: Forward (Down/South)
		AnimComp->AddAnimation(Animation{
			static_cast<uint8>(EIdleDirection::Forward), 1,
			vec2u{0, 0}, vec2u{0, 7},  // Row 0, frames 0-7
			});

		// Row 1: Forward-Right (Southeast)
		AnimComp->AddAnimation(Animation{
			static_cast<uint8>(EIdleDirection::ForwardRight), 1,
			vec2u{1, 0}, vec2u{1, 7},  // Row 1, frames 0-7
			});

		// Row 2: Right (East)
		AnimComp->AddAnimation(Animation{
			static_cast<uint8>(EIdleDirection::Right), 1,
			vec2u{2, 0}, vec2u{2, 7},
			});

		// Row 3: Back-Right (Northeast)
		AnimComp->AddAnimation(Animation{
			static_cast<uint8>(EIdleDirection::BackRight), 1,
			vec2u{3, 0}, vec2u{3, 7},
			});

		// Row 4: Back (North)
		AnimComp->AddAnimation(Animation{
			static_cast<uint8>(EIdleDirection::Back), 1,
			vec2u{4, 0}, vec2u{4, 7},
			});

		// Row 5: Back-Left (Northwest)
		AnimComp->AddAnimation(Animation{
			static_cast<uint8>(EIdleDirection::BackLeft), 1,
			vec2u{5, 0}, vec2u{5, 7},
			});

		// Row 6: Left (West)
		AnimComp->AddAnimation(Animation{
			static_cast<uint8>(EIdleDirection::Left), 1,
			vec2u{6, 0}, vec2u{6, 7},
			});

		// Row 7: Forward-Left (Southwest)
		AnimComp->AddAnimation(Animation{
			static_cast<uint8>(EIdleDirection::ForwardLeft), 1,
			vec2u{7, 0}, vec2u{7, 7},
			});

		// Start with forward-facing idle
		AnimComp->Transition(static_cast<uint8>(EIdleDirection::Forward));
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
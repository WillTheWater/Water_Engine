// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Player/PlayerCharacter.h"
#include "Component/AnimationComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent.h"
#include "Interaction/IInteractable.h"
#include "Input/InputActions.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Utility/Log.h"
#include <cmath>

namespace we
{
	PlayerCharacter::PlayerCharacter(World& OwningWorld)
		: Character(OwningWorld)
	{
	}

	PlayerCharacter::~PlayerCharacter() = default;

	void PlayerCharacter::BeginPlay()
	{
		PhysicsComp->SetBodyType(b2_dynamicBody);
		PhysicsComp->SetShapeType(PhysicsComponent::EShapeType::Circle);
		PhysicsComp->SetShapeSize({ 42.0f, 42.0f });
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

	void PlayerCharacter::SetupAnimations()
	{
		SpriteSheet IdleSheet("Assets/Textures/Game/idle.png", vec2u{ 100, 128 }, 8);
		AnimComp->AddSpriteSheet(EPlayerState::Idle, IdleSheet);

		SpriteSheet WalkSheet("Assets/Textures/Game/walk.png", vec2u{ 100, 128 }, 8);
		AnimComp->AddSpriteSheet(EPlayerState::Walk, WalkSheet);

		// 8-way idle animations
		AnimComp->AddAnimation(Animation{ EPlayerAnim::IdleForward, EPlayerState::Idle, vec2u{0, 0}, vec2u{0, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::IdleForwardRight, EPlayerState::Idle, vec2u{1, 0}, vec2u{1, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::IdleRight, EPlayerState::Idle, vec2u{2, 0}, vec2u{2, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::IdleBackRight, EPlayerState::Idle, vec2u{3, 0}, vec2u{3, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::IdleBack, EPlayerState::Idle, vec2u{4, 0}, vec2u{4, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::IdleBackLeft, EPlayerState::Idle, vec2u{5, 0}, vec2u{5, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::IdleLeft, EPlayerState::Idle, vec2u{6, 0}, vec2u{6, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::IdleForwardLeft, EPlayerState::Idle, vec2u{7, 0}, vec2u{7, 7} });

		// 8-way walk animations
		AnimComp->AddAnimation(Animation{ EPlayerAnim::WalkForward, EPlayerState::Walk, vec2u{0, 0}, vec2u{0, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::WalkForwardRight, EPlayerState::Walk, vec2u{1, 0}, vec2u{1, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::WalkRight, EPlayerState::Walk, vec2u{2, 0}, vec2u{2, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::WalkBackRight, EPlayerState::Walk, vec2u{3, 0}, vec2u{3, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::WalkBack, EPlayerState::Walk, vec2u{4, 0}, vec2u{4, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::WalkBackLeft, EPlayerState::Walk, vec2u{5, 0}, vec2u{5, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::WalkLeft, EPlayerState::Walk, vec2u{6, 0}, vec2u{6, 7} });
		AnimComp->AddAnimation(Animation{ EPlayerAnim::WalkForwardLeft, EPlayerState::Walk, vec2u{7, 0}, vec2u{7, 7} });

		// Start with forward-facing idle
		AnimComp->TransitionTo(EPlayerAnim::IdleForward);
	}

	void PlayerCharacter::Tick(float DeltaTime)
	{
		HandleInput();
		UpdateDirectionalAnimation();

		Character::Tick(DeltaTime);
	}

	void PlayerCharacter::EndPlay()
	{
		Character::EndPlay();
	}

	void PlayerCharacter::BindInput()
	{
		auto& Input = InputController();

		Input.Bind(MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::W });
		Input.Bind(MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::S });
		Input.Bind(MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::A });
		Input.Bind(MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::D });

		Input.Bind(MOVE_UP, Input::Keyboard{ sf::Keyboard::Scan::Up });
		Input.Bind(MOVE_DOWN, Input::Keyboard{ sf::Keyboard::Scan::Down });
		Input.Bind(MOVE_LEFT, Input::Keyboard{ sf::Keyboard::Scan::Left });
		Input.Bind(MOVE_RIGHT, Input::Keyboard{ sf::Keyboard::Scan::Right });

		Input.Bind(ACTION_INTERACT, Input::Keyboard{ sf::Keyboard::Scan::E });
		InteractBinding = Input.BindAction(ACTION_INTERACT, this, &PlayerCharacter::TryInteract);
	}

	void PlayerCharacter::HandleInput()
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

	void PlayerCharacter::TryInteract()
	{
		if (!CollComp)
		{
			LOG("[PlayerCharacter] No collision component");
			return;
		}

		for (Actor* Other : CollComp->GetOtherActors())
		{
			if (!Other)
				continue;

			if (auto* Interactable = dynamic_cast<IInteractable*>(Other))
			{
				if (Interactable->CanInteract(this))
				{
					LOG("[PlayerCharacter] Interacting with Actor {}", Other->GetID());
					Interactable->Interact(this);
					return;
				}
			}
		}

		LOG("[PlayerCharacter] Nothing to interact with");
	}

	void PlayerCharacter::UpdateDirectionalAnimation()
	{
		if (!AnimComp || !MoveComp) return;

		vec2f LastDir = MoveComp->GetLastMoveDirection();
		EPlayerAnim TargetAnim = MoveComp->IsMoving()
			? DirectionToWalkAnim(LastDir)
			: DirectionToIdleAnim(LastDir);

		if (!AnimComp->IsPlaying(static_cast<uint8>(TargetAnim)))
		{
			AnimComp->TransitionTo(TargetAnim);
		}
	}

	EPlayerAnim PlayerCharacter::DirectionToIdleAnim(const vec2f& Dir) const
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
		case 0: return EPlayerAnim::IdleBack;
		case 1: return EPlayerAnim::IdleBackRight;
		case 2: return EPlayerAnim::IdleRight;
		case 3: return EPlayerAnim::IdleForwardRight;
		case 4: return EPlayerAnim::IdleForward;
		case 5: return EPlayerAnim::IdleForwardLeft;
		case 6: return EPlayerAnim::IdleLeft;
		case 7: return EPlayerAnim::IdleBackLeft;
		default: return EPlayerAnim::IdleForward;
		}
	}

	EPlayerAnim PlayerCharacter::DirectionToWalkAnim(const vec2f& Dir) const
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
		case 0: return EPlayerAnim::WalkBack;
		case 1: return EPlayerAnim::WalkBackRight;
		case 2: return EPlayerAnim::WalkRight;
		case 3: return EPlayerAnim::WalkForwardRight;
		case 4: return EPlayerAnim::WalkForward;
		case 5: return EPlayerAnim::WalkForwardLeft;
		case 6: return EPlayerAnim::WalkLeft;
		case 7: return EPlayerAnim::WalkBackLeft;
		default: return EPlayerAnim::WalkForward;
		}
	}

	void PlayerCharacter::SetCameraBounds()
	{
		if (auto CamComp = GetCameraComponent())
		{
			// World bounds: (0,0) to (5760, 3240)
			constexpr vec2f WorldSize = { 5760.0f, 3240.0f };

			// Viewport size (1920x1080)
			constexpr vec2f ViewSize = { 1920.0f, 1080.0f };

			// Camera center clamping - viewport half-size from edges
			vec2f MinBounds = ViewSize / 2.0f;  // (960, 540)
			vec2f MaxBounds = WorldSize - MinBounds;  // (4800, 2700)

			CamComp->SetBounds({ MinBounds, MaxBounds - MinBounds });
		}
	}
}

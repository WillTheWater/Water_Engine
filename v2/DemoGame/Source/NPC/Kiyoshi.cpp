// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "NPC/Kiyoshi.h"
#include "Component/AnimationComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/MovementComponent.h"
#include "Tests/TestCharacter.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"
#include "Utility/Math.h"
#include <cmath>

namespace we
{
	Kiyoshi::Kiyoshi(World& OwningWorld)
		: Character(OwningWorld)
	{
	}

	Kiyoshi::~Kiyoshi() = default;

	void Kiyoshi::SetWaypoints(const vector<vec2f>& InWaypoints)
	{
		Waypoints = InWaypoints;
	}

	void Kiyoshi::Interact(Actor* Interactor)
	{
		LOG("[Kiyoshi] Hello traveler! *bows*");
	}

	void Kiyoshi::BeginPlay()
	{
		// Kinematic body - moves autonomously
		PhysicsComp->SetBodyType(b2_kinematicBody);
		PhysicsComp->SetShapeType(PhysicsComponent::EShapeType::Circle);
		PhysicsComp->SetShapeSize({42.0f, 42.0f});
		PhysicsComp->SetLinearDamping(10.0f);

		CollComp->SetRadius(64.0f);
		MoveComp->SetSpeed(120);

		Character::BeginPlay();

		SetupAnimations();
		
		// Start patrol
		if (!Waypoints.empty())
		{
			CurrentWaypointIndex = 0;
			SetPosition(Waypoints[0]);
			StartMovingToNextWaypoint();
		}
	}

	void Kiyoshi::StartMovingToNextWaypoint()
	{
		if (Waypoints.size() < 2)
			return;
		
		CurrentWaypointIndex = (CurrentWaypointIndex + 1) % Waypoints.size();
		AIState = EAIState::Moving;
	}

	void Kiyoshi::Tick(float DeltaTime)
	{
		bool bIsOverlappingPlayer = CheckPlayerOverlap();
		
		// Handle player interaction
		if (bIsOverlappingPlayer)
		{
			if (AIState != EAIState::Interacting)
			{
				// Just started overlapping - stop and face player
				AIState = EAIState::Interacting;
				MoveComp->ClearInput();
				FacePlayer();
			}
		}
		else if (bWasOverlappingPlayer && !bIsOverlappingPlayer)
		{
			// Player just left - start wait timer before resuming
			AIState = EAIState::Waiting;
			float WaitTime = RNG().Random(1.0f, 4.0f);
			WaitTimer = GetTimer().SetTimer(
				weak_from_this(),
				&Kiyoshi::OnResumeFromInteraction,
				WaitTime,
				false
			);
		}
		
		bWasOverlappingPlayer = bIsOverlappingPlayer;
		
		// Normal patrol behavior
		if (AIState == EAIState::Moving && !Waypoints.empty())
		{
			vec2f TargetPos = Waypoints[CurrentWaypointIndex];
			vec2f CurrentPos = GetPosition();
			vec2f ToTarget = TargetPos - CurrentPos;
			float DistSq = ToTarget.lengthSquared();
			
			if (DistSq < 25.0f)
			{
				// Reached waypoint - start waiting
				AIState = EAIState::Waiting;
				MoveComp->ClearInput();
				
				float WaitTime = RNG().Random(1.0f, 4.0f);
				WaitTimer = GetTimer().SetTimer(
					weak_from_this(),
					&Kiyoshi::OnWaitComplete,
					WaitTime,
					false
				);
			}
			else
			{
				// Move toward target
				vec2f Direction = ToTarget / std::sqrt(DistSq);
				MoveComp->AddInputVector(Direction);
			}
		}
		
		UpdateDirectionalAnimation();
		Character::Tick(DeltaTime);
	}

	bool Kiyoshi::CheckPlayerOverlap()
	{
		if (!CollComp->IsOverlapping())
			return false;
		
		return CollComp->GetOtherActor<TestCharacter>() != nullptr;
	}

	void Kiyoshi::FacePlayer()
	{
		if (!CollComp->IsOverlapping())
			return;
		
		// Find player in overlapping actors
		for (Actor* Other : CollComp->GetOtherActors())
		{
			if (dynamic_cast<TestCharacter*>(Other))
			{
				vec2f ToPlayer = Other->GetPosition() - GetPosition();
				if (ToPlayer.lengthSquared() > 0.001f)
				{
					MoveComp->SetLastMoveDirection(ToPlayer);
				}
				return;
			}
		}
	}

	void Kiyoshi::OnWaitComplete()
	{
		StartMovingToNextWaypoint();
	}

	void Kiyoshi::OnResumeFromInteraction()
	{
		AIState = EAIState::Moving;
	}

	void Kiyoshi::EndPlay()
	{
		GetTimer().ClearTimer(WaitTimer);
		Character::EndPlay();
	}

	void Kiyoshi::SetupAnimations()
	{
		SpriteSheet IdleSheet("Assets/Textures/Game/oldIdle.png", vec2u{256, 256}, 8);
		AnimComp->AddSpriteSheet(EKiyoshiSheet::Idle, IdleSheet);
		
		SpriteSheet WalkSheet("Assets/Textures/Game/oldWalk.png", vec2u{256, 256}, 8);
		AnimComp->AddSpriteSheet(EKiyoshiSheet::Walk, WalkSheet);

		// 8-way idle animations
		AnimComp->AddAnimation(Animation{EKiyoshiState::IdleForward,       EKiyoshiSheet::Idle, vec2u{0, 0}, vec2u{0, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::IdleForwardRight,  EKiyoshiSheet::Idle, vec2u{1, 0}, vec2u{1, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::IdleRight,         EKiyoshiSheet::Idle, vec2u{2, 0}, vec2u{2, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::IdleBackRight,     EKiyoshiSheet::Idle, vec2u{3, 0}, vec2u{3, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::IdleBack,          EKiyoshiSheet::Idle, vec2u{4, 0}, vec2u{4, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::IdleBackLeft,      EKiyoshiSheet::Idle, vec2u{5, 0}, vec2u{5, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::IdleLeft,          EKiyoshiSheet::Idle, vec2u{6, 0}, vec2u{6, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::IdleForwardLeft,   EKiyoshiSheet::Idle, vec2u{7, 0}, vec2u{7, 7}});

		// 8-way walk animations
		AnimComp->AddAnimation(Animation{EKiyoshiState::WalkForward,       EKiyoshiSheet::Walk, vec2u{0, 0}, vec2u{0, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::WalkForwardRight,  EKiyoshiSheet::Walk, vec2u{1, 0}, vec2u{1, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::WalkRight,         EKiyoshiSheet::Walk, vec2u{2, 0}, vec2u{2, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::WalkBackRight,     EKiyoshiSheet::Walk, vec2u{3, 0}, vec2u{3, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::WalkBack,          EKiyoshiSheet::Walk, vec2u{4, 0}, vec2u{4, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::WalkBackLeft,      EKiyoshiSheet::Walk, vec2u{5, 0}, vec2u{5, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::WalkLeft,          EKiyoshiSheet::Walk, vec2u{6, 0}, vec2u{6, 7}});
		AnimComp->AddAnimation(Animation{EKiyoshiState::WalkForwardLeft,   EKiyoshiSheet::Walk, vec2u{7, 0}, vec2u{7, 7}});

		AnimComp->TransitionTo(EKiyoshiState::IdleForward);
	}

	void Kiyoshi::UpdateDirectionalAnimation()
	{
		if (!AnimComp || !MoveComp)
			return;

		vec2f LastDir = MoveComp->GetLastMoveDirection();
		EKiyoshiState TargetAnim = MoveComp->IsMoving() 
			? DirectionToWalkAnim(LastDir) 
			: DirectionToIdleAnim(LastDir);
		
		if (!AnimComp->IsPlaying(TargetAnim))
		{
			AnimComp->TransitionTo(TargetAnim);
		}
	}

	EKiyoshiState Kiyoshi::DirectionToIdleAnim(const vec2f& Dir) const
	{
		vec2f N = Dir;
		float LenSq = N.lengthSquared();
		if (LenSq > 0.0f)
			N /= std::sqrt(LenSq);

		float Angle = std::atan2(N.y, N.x);
		float Degrees = Angle * 180.0f / 3.14159f;
		
		float Normalized = Degrees + 90.0f;
		if (Normalized < 0.0f) Normalized += 360.0f;
		if (Normalized >= 360.0f) Normalized -= 360.0f;
		
		int Sector = static_cast<int>((Normalized + 22.5f) / 45.0f) % 8;
		
		switch (Sector)
		{
			case 0: return EKiyoshiState::IdleBack;
			case 1: return EKiyoshiState::IdleBackRight;
			case 2: return EKiyoshiState::IdleRight;
			case 3: return EKiyoshiState::IdleForwardRight;
			case 4: return EKiyoshiState::IdleForward;
			case 5: return EKiyoshiState::IdleForwardLeft;
			case 6: return EKiyoshiState::IdleLeft;
			case 7: return EKiyoshiState::IdleBackLeft;
			default: return EKiyoshiState::IdleForward;
		}
	}

	EKiyoshiState Kiyoshi::DirectionToWalkAnim(const vec2f& Dir) const
	{
		vec2f N = Dir;
		float LenSq = N.lengthSquared();
		if (LenSq > 0.0f)
			N /= std::sqrt(LenSq);

		float Angle = std::atan2(N.y, N.x);
		float Degrees = Angle * 180.0f / 3.14159f;
		
		float Normalized = Degrees + 90.0f;
		if (Normalized < 0.0f) Normalized += 360.0f;
		if (Normalized >= 360.0f) Normalized -= 360.0f;
		
		int Sector = static_cast<int>((Normalized + 22.5f) / 45.0f) % 8;
		
		switch (Sector)
		{
			case 0: return EKiyoshiState::WalkBack;
			case 1: return EKiyoshiState::WalkBackRight;
			case 2: return EKiyoshiState::WalkRight;
			case 3: return EKiyoshiState::WalkForwardRight;
			case 4: return EKiyoshiState::WalkForward;
			case 5: return EKiyoshiState::WalkForwardLeft;
			case 6: return EKiyoshiState::WalkLeft;
			case 7: return EKiyoshiState::WalkBackLeft;
			default: return EKiyoshiState::WalkForward;
		}
	}
}
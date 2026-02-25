// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/Kiyoshi.h"
#include "Interface/Component/AnimationComponent.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	Kiyoshi::Kiyoshi(World* OwningWorld)
		: NPC(OwningWorld, GC.OldNPCSheetIdle, "Kiyoshi")
	{
		// Set up paginated dialog - player presses Continue to advance
		SetDialogPages({
			"Greetings, traveler! I am Kiyoshi, the village merchant.",
			"I've been selling potions and rare items in these parts for over forty years.",
			"If you ever need healing potions or magical artifacts, come find me!",
			"Is there something I can help you with today?"
		});
	}

	void Kiyoshi::BeginPlay()
	{
		NPC::BeginPlay();
		
		if (PhysicsComp)
		{
			PhysicsComp->SetBodyType(BodyType::Kinematic);
		}
		
		SetupWaypoints();
		
		StartIdling();
	}

	void Kiyoshi::SetupWaypoints()
	{
		// 3 waypoints
		vec2f StartPos = GetPosition();
		
		Waypoints.push_back(StartPos);
		Waypoints.push_back(StartPos + vec2f{ 400.f, 0.f });
		Waypoints.push_back(StartPos + vec2f{ 100.f, 350.f });
		
		CurrentWaypointIndex = 0;
	}

	void Kiyoshi::Tick(float DeltaTime)
	{
		if (AnimComp) AnimComp->Tick(DeltaTime);
		
		if (!bInteracting)
		{
			UpdateMovement(DeltaTime);
		}
		
		UpdateAnimation();
		
		SetVelocity(CurrentVelocity);
		
		NPC::Tick(DeltaTime);
	}

	void Kiyoshi::UpdateMovement(float DeltaTime)
	{
		if (CurrentState == KiyoshiState::Walking)
		{
			vec2f CurrentPos = GetPosition();
			vec2f TargetPos = Waypoints[CurrentWaypointIndex];
			vec2f ToTarget = TargetPos - CurrentPos;
			float DistanceSquared = ToTarget.lengthSquared();
			
			if (DistanceSquared < 100.0f)
			{
				StartIdling();
			}
			else
			{
				vec2f Direction = ToTarget.normalized();
				CurrentVelocity = Direction * WalkSpeed;
				SetFacingDirection(Direction);
			}
		}
	}

	void Kiyoshi::StartIdling()
	{
		CurrentState = KiyoshiState::Idle;
		CurrentVelocity = vec2f{ 0, 0 };
		
		float IdleTime = RandomRange(MinIdleTime, MaxIdleTime);
		
		IdleTimerHandle = TimerManager::Get().SetTimer(
			std::static_pointer_cast<Kiyoshi>(shared_from_this()), 
			&Kiyoshi::OnIdleTimerExpired, 
			IdleTime, 
			false
		);
	}

	void Kiyoshi::OnIdleTimerExpired()
	{
		if (!bInteracting)
		{
			StartWalking();
		}
	}

	void Kiyoshi::StartWalking()
	{
		CurrentState = KiyoshiState::Walking;
		
		MoveToNextWaypoint();
	}

	void Kiyoshi::MoveToNextWaypoint()
	{
		CurrentWaypointIndex = (CurrentWaypointIndex + 1) % Waypoints.size();
		
		vec2f TargetPos = Waypoints[CurrentWaypointIndex];
		vec2f CurrentPos = GetPosition();
		
		vec2f Direction = (TargetPos - CurrentPos).normalized();
		
		CurrentVelocity = Direction * WalkSpeed;
		SetFacingDirection(Direction);
	}

	void Kiyoshi::SetFacingDirection(const vec2f& Direction)
	{
		if (Direction.lengthSquared() < 0.01f) return;
		ForwardVector = Direction.normalized();
	}

	void Kiyoshi::FacePosition(const vec2f& TargetPos)
	{
		vec2f CurrentPos = GetPosition();
		vec2f Direction = TargetPos - CurrentPos;
		SetFacingDirection(Direction);
	}

	void Kiyoshi::UpdateAnimation()
	{
		if (!AnimComp) return;
		
		bool bIsMoving = (CurrentVelocity.lengthSquared() > 1.0f);
		
		float Angle = std::atan2(ForwardVector.y, ForwardVector.x);
		int DirIndex = GetDirectionIndexFromAngle(Angle);
		
		uint8 BaseState = bIsMoving ? 9 : 1;
		uint8 TargetState = BaseState + DirIndex;
		
		AnimComp->Transition(TargetState);
	}

	int Kiyoshi::GetDirectionIndexFromAngle(float Angle) const
	{
		int DirIndex = int(std::round(Angle / (PI / 4.0f))) % 8;
		if (DirIndex < 0) DirIndex += 8;
				
		static const int Remap[8] = {
			2,  // Right (0) -> index 2
			1,  // DownRight (1) -> index 1
			0,  // Down (2) -> index 0
			7,  // DownLeft (3) -> index 7
			6,  // Left (4) -> index 6
			5,  // UpLeft (5) -> index 5
			4,  // Up (6) -> index 4
			3   // UpRight (7) -> index 3
		};
		
		return Remap[DirIndex];
	}

	void Kiyoshi::OnInteract(Actor* Interactor)
	{
		NPC::OnInteract(Interactor);
		
		if (!bInteracting)
		{
			bInteracting = true;
			TimerManager::Get().ClearTimer(IdleTimerHandle);
			CurrentState = KiyoshiState::Idle;
			CurrentVelocity = vec2f{ 0, 0 };
			
			if (Interactor)
			{
				FacePosition(Interactor->GetPosition());
				UpdateAnimation();
			}
		}
		else
		{
			bInteracting = false;
		}
	}

	void Kiyoshi::OnPlayerEnteredRange(Actor* Player)
	{
		if (!bInteracting && CurrentState == KiyoshiState::Walking)
		{
			TimerManager::Get().ClearTimer(IdleTimerHandle);
			StartIdling();
			if (Player)
			{
				FacePosition(Player->GetPosition());
				UpdateAnimation();
			}
		}
		(void)Player;
	}

	void Kiyoshi::OnPlayerLeftRange(Actor* Player)
	{
		if (IsDialogVisible())
		{
			HideDialog();
		}
		if (bInteracting)
		{
			bInteracting = false;
		}
		ResumePatrol();
		(void)Player;
	}

	void Kiyoshi::ResumePatrol()
	{
		bInteracting = false;
		CurrentState = KiyoshiState::Idle;
		CurrentVelocity = vec2f{ 0, 0 };
		StartIdling();
	}

	void Kiyoshi::Destroy()
	{
		TimerManager::Get().ClearTimer(IdleTimerHandle);
		
		NPC::Destroy();
	}

	void Kiyoshi::InitializeAnimations()
	{
		AnimComp = make_shared<AnimationComponent>(this);
		AnimComp->BeginPlay();

		AnimComp->AddSpriteSheet(0, SpriteSheet{ GC.OldNPCSheetIdle, {256, 256}, 8 });
		
		AnimComp->AddSpriteSheet(1, SpriteSheet{ GC.OldNPCSheetWalk, {256, 256}, 8 });

		// IDLE animations (states 1-8)
		AnimComp->AddAnimation({ 1, 0, {0, 0}, {0, 7}, 0.15f, true });  // Down
		AnimComp->AddAnimation({ 2, 0, {1, 0}, {1, 7}, 0.15f, true });  // DownRight
		AnimComp->AddAnimation({ 3, 0, {2, 0}, {2, 7}, 0.15f, true });  // Right
		AnimComp->AddAnimation({ 4, 0, {3, 0}, {3, 7}, 0.15f, true });  // UpRight
		AnimComp->AddAnimation({ 5, 0, {4, 0}, {4, 7}, 0.15f, true });  // Up
		AnimComp->AddAnimation({ 6, 0, {5, 0}, {5, 7}, 0.15f, true });  // UpLeft
		AnimComp->AddAnimation({ 7, 0, {6, 0}, {6, 7}, 0.15f, true });  // Left
		AnimComp->AddAnimation({ 8, 0, {7, 0}, {7, 7}, 0.15f, true });  // DownLeft

		// WALK animations (states 9-16)
		AnimComp->AddAnimation({ 9,  1, {0, 0}, {0, 7}, 0.30f, true });  // Down
		AnimComp->AddAnimation({ 10, 1, {1, 0}, {1, 7}, 0.30f, true });  // DownRight
		AnimComp->AddAnimation({ 11, 1, {2, 0}, {2, 7}, 0.30f, true });  // Right
		AnimComp->AddAnimation({ 12, 1, {3, 0}, {3, 7}, 0.30f, true });  // UpRight
		AnimComp->AddAnimation({ 13, 1, {4, 0}, {4, 7}, 0.30f, true });  // Up
		AnimComp->AddAnimation({ 14, 1, {5, 0}, {5, 7}, 0.30f, true });  // UpLeft
		AnimComp->AddAnimation({ 15, 1, {6, 0}, {6, 7}, 0.30f, true });  // Left
		AnimComp->AddAnimation({ 16, 1, {7, 0}, {7, 7}, 0.30f, true });  // DownLeft

		// Start facing down (idle)
		AnimComp->Transition(1);
	}
}

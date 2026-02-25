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
		SetDialog("Greetings! I'm Kiyoshi. I sell potions and items.");
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
		// Set up 3 waypoints for Kiyoshi to patrol between
		vec2f StartPos = GetPosition();
		
		Waypoints.push_back(StartPos);
		Waypoints.push_back(StartPos + vec2f{ 400.f, 0.f });   // 200 units right
		Waypoints.push_back(StartPos + vec2f{ 100.f, 350.f }); // 100 right, 150 down
		
		CurrentWaypointIndex = 0;
		
		LOG("[Kiyoshi] Setup {} waypoints", Waypoints.size());
	}

	void Kiyoshi::Tick(float DeltaTime)
	{
		// Update animation component
		if (AnimComp) AnimComp->Tick(DeltaTime);
		
		// Update movement logic (only if not interacting)
		if (!bInteracting)
		{
			UpdateMovement(DeltaTime);
		}
		
		// Update animation based on state
		UpdateAnimation();
		
		// Apply velocity through physics component
		SetVelocity(CurrentVelocity);
		
		// Update base NPC (UI, etc.)
		NPC::Tick(DeltaTime);
	}

	void Kiyoshi::UpdateMovement(float DeltaTime)
	{
		if (CurrentState == KiyoshiState::Walking)
		{
			// Check if we've reached the waypoint
			vec2f CurrentPos = GetPosition();
			vec2f TargetPos = Waypoints[CurrentWaypointIndex];
			vec2f ToTarget = TargetPos - CurrentPos;
			float DistanceSquared = ToTarget.lengthSquared();
			
			// If close enough to waypoint, stop and idle
			if (DistanceSquared < 100.0f)  // 10 units * 10 units
			{
				StartIdling();
			}
			else
			{
				// Continue moving toward waypoint
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
		
		// Set random idle time between 3-6 seconds
		float IdleTime = RandomRange(MinIdleTime, MaxIdleTime);
		
		IdleTimerHandle = TimerManager::Get().SetTimer(
			std::static_pointer_cast<Kiyoshi>(shared_from_this()), 
			&Kiyoshi::OnIdleTimerExpired, 
			IdleTime, 
			false
		);
		
		LOG("[Kiyoshi] Starting idle for {:.2f} seconds at waypoint {}", IdleTime, CurrentWaypointIndex);
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
		
		// Move to next waypoint
		MoveToNextWaypoint();
		
		LOG("[Kiyoshi] Starting walk to waypoint {}", CurrentWaypointIndex);
	}

	void Kiyoshi::MoveToNextWaypoint()
	{
		// Advance to next waypoint
		CurrentWaypointIndex = (CurrentWaypointIndex + 1) % Waypoints.size();
		
		vec2f TargetPos = Waypoints[CurrentWaypointIndex];
		vec2f CurrentPos = GetPosition();
		
		// Calculate direction to waypoint
		vec2f Direction = (TargetPos - CurrentPos).normalized();
		
		// Set velocity and facing
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
		
		// Determine if moving
		bool bIsMoving = (CurrentVelocity.lengthSquared() > 1.0f);
		
		// Get angle from forward vector
		float Angle = std::atan2(ForwardVector.y, ForwardVector.x);
		int DirIndex = GetDirectionIndexFromAngle(Angle);
		
		// Animation state IDs:
		// 1-8 = Idle (down, downright, right, upright, up, upleft, left, downleft)
		// 9-16 = Walk (down, downright, right, upright, up, upleft, left, downleft)
		
		uint8 BaseState = bIsMoving ? 9 : 1;
		uint8 TargetState = BaseState + DirIndex;
		
		AnimComp->Transition(TargetState);
	}

	int Kiyoshi::GetDirectionIndexFromAngle(float Angle) const
	{
		// Convert angle to 8-direction index
		int DirIndex = int(std::round(Angle / (PI / 4.0f))) % 8;
		if (DirIndex < 0) DirIndex += 8;
		
		// Remap to animation order (down, downright, right, upright, up, upleft, left, downleft)
		// Angle order: 0=Right, 1=DownRight, 2=Down, 3=DownLeft, 4=Left, 5=UpLeft, 6=Up, 7=UpRight
		// Our order:   0=Down, 1=DownRight, 2=Right, 3=UpRight, 4=Up, 5=UpLeft, 6=Left, 7=DownLeft
		
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
		// Call base NPC logic (show/hide dialog)
		NPC::OnInteract(Interactor);
		
		if (!bInteracting)
		{
			// Start interacting - stop moving
			bInteracting = true;
			
			// Cancel any idle timer
			TimerManager::Get().ClearTimer(IdleTimerHandle);
			
			// Stop movement
			CurrentState = KiyoshiState::Idle;
			CurrentVelocity = vec2f{ 0, 0 };
			
			// Face the player IMMEDIATELY
			if (Interactor)
			{
				FacePosition(Interactor->GetPosition());
				// Update animation immediately to show facing
				UpdateAnimation();
			}
			
			LOG("[Kiyoshi] Started interacting with {}", Interactor ? typeid(*Interactor).name() : "unknown");
		}
		else
		{
			// Dialog closed - DON'T resume patrol here
			// Wait until player actually leaves
			bInteracting = false;
			
			LOG("[Kiyoshi] Dialog closed, waiting for player to leave");
		}
	}

	void Kiyoshi::OnPlayerEnteredRange(Actor* Player)
	{
		// When player enters range, stop walking and idle
		if (!bInteracting && CurrentState == KiyoshiState::Walking)
		{
			// Cancel any existing timer
			TimerManager::Get().ClearTimer(IdleTimerHandle);
			
			// Stop and idle
			StartIdling();
			
			// Face the player
			if (Player)
			{
				FacePosition(Player->GetPosition());
				UpdateAnimation();
			}
			
			LOG("[Kiyoshi] Player entered range, stopping to idle");
		}
		(void)Player;
	}

	void Kiyoshi::OnPlayerLeftRange(Actor* Player)
	{
		// When player leaves, hide dialog and resume patrol
		if (IsDialogVisible())
		{
			HideDialog();
		}
		
		// Always resume patrol when player leaves
		if (bInteracting)
		{
			bInteracting = false;
		}
		
		// Resume patrol
		ResumePatrol();
		
		LOG("[Kiyoshi] Player left range, resuming patrol");
		(void)Player;
	}

	void Kiyoshi::ResumePatrol()
	{
		// Make sure we're not interacting and set to idle state
		bInteracting = false;
		CurrentState = KiyoshiState::Idle;
		CurrentVelocity = vec2f{ 0, 0 };
		
		// Start idling with a timer to resume walking
		StartIdling();
	}

	void Kiyoshi::Destroy()
	{
		// Clear timer
		TimerManager::Get().ClearTimer(IdleTimerHandle);
		
		NPC::Destroy();
	}

	void Kiyoshi::InitializeAnimations()
	{
		AnimComp = make_shared<AnimationComponent>(this);
		AnimComp->BeginPlay();

		// IDLE sprite sheet (oldIdle - 8x8 grid)
		AnimComp->AddSpriteSheet(0, SpriteSheet{ GC.OldNPCSheetIdle, {256, 256}, 8 });
		
		// WALK sprite sheet (oldWalk - 8x8 grid)
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
		
		LOG("[Kiyoshi] Initialized 8-way idle and walk animations");
	}
}

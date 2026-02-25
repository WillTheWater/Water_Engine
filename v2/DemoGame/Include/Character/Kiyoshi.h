// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Character/NPC.h"
#include "Utility/Timer.h"

namespace we
{
	// =============================================================================
	// Kiyoshi - Old Man Merchant NPC with 8-way movement and waypoint patrol
	// =============================================================================

	enum class KiyoshiState : uint8
	{
		Idle,
		Walking
	};

	class Kiyoshi : public NPC
	{
	public:
		Kiyoshi(World* OwningWorld);

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Destroy() override;
		virtual void OnInteract(Actor* Interactor) override;
		virtual void OnPlayerEnteredRange(Actor* Player) override;
		virtual void OnPlayerLeftRange(Actor* Player) override;

	protected:
		virtual void InitializeAnimations() override;

	private:
		void SetupWaypoints();
		void UpdateMovement(float DeltaTime);
		void UpdateAnimation();
		void MoveToNextWaypoint();
		void StartIdling();
		void StartWalking();
		void FacePosition(const vec2f& TargetPos);
		void SetFacingDirection(const vec2f& Direction);
		int GetDirectionIndexFromAngle(float Angle) const;
		void ResumePatrol();
		void OnIdleTimerExpired();

	private:
		// Waypoint system
		vector<vec2f> Waypoints;
		uint CurrentWaypointIndex = 0;
		
		// State
		KiyoshiState CurrentState = KiyoshiState::Idle;
		bool bInteracting = false;
		
		// Movement
		vec2f CurrentVelocity;
		vec2f ForwardVector{ 0, 1 };  // Default facing down
		float WalkSpeed = 80.0f;
		
		// Idle timer
		TimerHandle IdleTimerHandle;
		float MinIdleTime = 2.0f;
		float MaxIdleTime = 4.0f;
	};
}

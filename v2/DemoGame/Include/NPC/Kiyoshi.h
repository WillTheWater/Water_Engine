// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Character.h"
#include "Subsystem/TimerSubsystem.h"
#include "Interaction/IInteractable.h"
#include "UI/InteractUI.h"
#include "UI/DialogUI.h"

namespace we
{
	enum class EKiyoshiState : uint8
	{
		// Idle animations
		IdleForward,
		IdleForwardRight,
		IdleRight,
		IdleBackRight,
		IdleBack,
		IdleBackLeft,
		IdleLeft,
		IdleForwardLeft,
		
		// Walk animations
		WalkForward,
		WalkForwardRight,
		WalkRight,
		WalkBackRight,
		WalkBack,
		WalkBackLeft,
		WalkLeft,
		WalkForwardLeft
	};

	enum class EKiyoshiSheet : uint8
	{
		Idle,
		Walk
	};

	enum class EAIState : uint8
	{
		Moving,
		Waiting,
		Interacting
	};

	class Kiyoshi : public Character, public IInteractable
	{
	public:
		Kiyoshi(World& OwningWorld);
		~Kiyoshi();

		// Configure patrol path
		void SetWaypoints(const vector<vec2f>& InWaypoints);

		// IInteractable
		void Interact(Actor* Interactor) override;
		void ShowPrompt(Actor* Interactor) override;
		void HidePrompt(Actor* Interactor) override;

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		void GetDrawables(vector<const drawable*>& OutDrawables) const override;

	private:
		void SetupAnimations();
		void UpdateDirectionalAnimation();
		EKiyoshiState DirectionToIdleAnim(const vec2f& Dir) const;
		EKiyoshiState DirectionToWalkAnim(const vec2f& Dir) const;
		
		void StartMovingToNextWaypoint();
		void OnWaitComplete();
		void OnResumeFromInteraction();
		void FacePlayer();
		void SetupShadow();
		void StartDialog(class PlayerCharacter* Player);
		bool AdvanceDialog();
		void EndDialog();

	private:
		// Patrol
		vector<vec2f> Waypoints;
		size_t CurrentWaypointIndex = 0;
		EAIState AIState = EAIState::Waiting;
		TimerHandle WaitTimer;
		Actor* CurrentInteractor = nullptr;
		InteractUI PromptUI;
		DialogUI DialogBox;
		bool bInDialog = false;

		// Shadow
		optional<sprite> ShadowSprite;
		shared<texture> ShadowTexture;
		vec2f ShadowOffset{ 0.0f, 95.0f };
	};
}
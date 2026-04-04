// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Character.h"
#include "Subsystem/InputSubsystem.h"
#include "Interaction/IInteractable.h"
#include "Interaction/QuestComponent.h"

namespace we
{
	enum class EPlayerAnim : uint8
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

	enum class EPlayerState : uint8
	{
		Idle,
		Walk
	};

	class PlayerCharacter : public Character
	{
	public:
		PlayerCharacter(World& OwningWorld);
		~PlayerCharacter();

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		void GetDrawables(vector<const drawable*>& OutDrawables) const override;

	private:
		void SetupAnimations();
		void BindInput();
		void HandleInput();
		void UpdateDirectionalAnimation();
		void TryInteract();
		void OnBeginOverlap(Actor* Other);
		void OnEndOverlap(Actor* Other);
		void StartInteraction(IInteractable* Target);
		void EndInteraction();
		EPlayerAnim DirectionToIdleAnim(const vec2f& Dir) const;
		EPlayerAnim DirectionToWalkAnim(const vec2f& Dir) const;
		void SetCameraBounds();
		void SetupShadow();
		void FaceInteractionTarget(Actor* Target);

		BindingHandle InteractBinding;
		
		// Interaction state
		IInteractable* CurrentInteractable = nullptr;
		bool bInDialog = false;
		bool bInputBlocked = false;

		// Shadow
		optional<sprite> ShadowSprite;
		shared<texture> ShadowTexture;
		vec2f ShadowOffset{ 0.0f, 95.0f };

	public:
		QuestComponent& GetQuest() { return QuestComp; }
		const QuestComponent& GetQuest() const { return QuestComp; }
		void SetInputBlocked(bool bBlocked) { bInputBlocked = bBlocked; }

	private:
		QuestComponent QuestComp;
	};
}
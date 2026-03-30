// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Character.h"
#include "Interaction/IInteractable.h"
#include "UI/InteractUI.h"
#include "UI/DialogUI.h"


namespace we
{
	class PlayerCharacter;

	enum class EAoiState : uint8
	{
		IdleForward,
		IdleForwardRight,
		IdleRight,
		IdleBackRight,
		IdleBack,
		IdleBackLeft,
		IdleLeft,
		IdleForwardLeft
	};

	enum class EAoiSheet : uint8
	{
		Idle
	};

	class AoiMizukawa : public Character, public IInteractable
	{
	public:
		AoiMizukawa(World& OwningWorld);
		~AoiMizukawa();

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
		void SetupAnimation();
		void UpdateDirectionalAnimation();
		EAoiState DirectionToAnim(const vec2f& Dir) const;
		void FacePlayer();
		void StartDialog(PlayerCharacter* Player);
		bool AdvanceDialog();
		void EndDialog();
		void SetupShadow();

	private:
		InteractUI PromptUI;
		DialogUI DialogBox;
		bool bInDialog = false;
		Actor* CurrentInteractor = nullptr;
		vec2f FacingDirection{ 0.0f, 1.0f };
		vec2f OriginalFacingDirection{ 0.0f, 1.0f };

		// Shadow
		optional<sprite> ShadowSprite;
		shared<texture> ShadowTexture;
		vec2f ShadowOffset{ 0.0f, 95.0f };
	};
}
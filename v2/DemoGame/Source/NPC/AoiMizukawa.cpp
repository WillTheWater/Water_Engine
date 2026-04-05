// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "NPC/AoiMizukawa.h"
#include "Component/AnimationComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/CollisionComponent.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Player/PlayerCharacter.h"
#include "Utility/Log.h"
#include "Utility/Math.h"

namespace we
{

	AoiMizukawa::AoiMizukawa(World& OwningWorld)
		: Character(OwningWorld)
	{
	}

	AoiMizukawa::~AoiMizukawa() = default;

	void AoiMizukawa::Interact(Actor* Interactor)
	{
		auto* Player = dynamic_cast<PlayerCharacter*>(Interactor);
		if (!Player) return;

		if (!bInDialog)
		{
			StartDialog(Player);
		}
		else
		{
			if (!AdvanceDialog())
			{
				EndDialog();
			}
		}
	}

	void AoiMizukawa::BeginPlay()
	{
		PhysicsComp->SetBodyType(b2_staticBody);
		PhysicsComp->SetShapeType(PhysicsComponent::EShapeType::Circle);
		PhysicsComp->SetShapeSize({42.0f, 42.0f});
		PhysicsComp->SetShapeOffset({ 0, 60 });

		CollComp->SetRadius(64.0f);
		CollComp->SetCollisionChannel(ECollisionChannel::Interaction);

		Character::BeginPlay();

		SetScale({ 2.1,2.1 });
		SetupAnimation();
		SetupShadow();

		PromptUI.Initialize("Talk");
		PromptUI.SetPosition(GetPosition(), {0.f, -100.f});
		
		DialogBox.Initialize();
		DialogBox.SetPosition(GetPosition(), {0.f, -200.f});
	}

	void AoiMizukawa::Tick(float DeltaTime)
	{
		UpdateDirectionalAnimation();
		Character::Tick(DeltaTime);
		if (ShadowSprite)
		{
			ShadowSprite->setPosition(GetPosition() + ShadowOffset);
		}
	}

	void AoiMizukawa::EndPlay()
	{
		Character::EndPlay();
	}

	void AoiMizukawa::SetupAnimation()
	{
		SpriteSheet IdleSheet("Assets/Textures/Game/girlidle.png", vec2u{ 100, 128 }, 8);
		AnimComp->AddSpriteSheet(EAoiSheet::Idle, IdleSheet);

		// 8-way idle animations (rows 0-7)
		AnimComp->AddAnimation(Animation{EAoiState::IdleForward,      EAoiSheet::Idle, vec2u{0, 0}, vec2u{0, 7}});
		AnimComp->AddAnimation(Animation{EAoiState::IdleForwardRight, EAoiSheet::Idle, vec2u{1, 0}, vec2u{1, 7}});
		AnimComp->AddAnimation(Animation{EAoiState::IdleRight,        EAoiSheet::Idle, vec2u{2, 0}, vec2u{2, 7}});
		AnimComp->AddAnimation(Animation{EAoiState::IdleBackRight,    EAoiSheet::Idle, vec2u{3, 0}, vec2u{3, 7}});
		AnimComp->AddAnimation(Animation{EAoiState::IdleBack,         EAoiSheet::Idle, vec2u{4, 0}, vec2u{4, 7}});
		AnimComp->AddAnimation(Animation{EAoiState::IdleBackLeft,     EAoiSheet::Idle, vec2u{5, 0}, vec2u{5, 7}});
		AnimComp->AddAnimation(Animation{EAoiState::IdleLeft,         EAoiSheet::Idle, vec2u{6, 0}, vec2u{6, 7}});
		AnimComp->AddAnimation(Animation{EAoiState::IdleForwardLeft,  EAoiSheet::Idle, vec2u{7, 0}, vec2u{7, 7}});

		// Play default idle animation
		AnimComp->TransitionTo(EAoiState::IdleForward);
	}

	void AoiMizukawa::UpdateDirectionalAnimation()
	{
		if (!AnimComp)
			return;

		EAoiState TargetAnim = DirectionToAnim(FacingDirection);
		if (!AnimComp->IsPlaying(TargetAnim))
		{
			AnimComp->TransitionTo(TargetAnim);
		}
	}

	EAoiState AoiMizukawa::DirectionToAnim(const vec2f& Dir) const
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
			case 0: return EAoiState::IdleBack;
			case 1: return EAoiState::IdleBackRight;
			case 2: return EAoiState::IdleRight;
			case 3: return EAoiState::IdleForwardRight;
			case 4: return EAoiState::IdleForward;
			case 5: return EAoiState::IdleForwardLeft;
			case 6: return EAoiState::IdleLeft;
			case 7: return EAoiState::IdleBackLeft;
			default: return EAoiState::IdleForward;
		}
	}

	void AoiMizukawa::FacePlayer()
	{
		if (!CurrentInteractor)
			return;

		vec2f ToInteractor = CurrentInteractor->GetPosition() - GetPosition();
		if (LengthSquared(ToInteractor) > EPSILON)
		{
			FacingDirection = ToInteractor / std::sqrt(LengthSquared(ToInteractor));
		}
	}

	void AoiMizukawa::ShowPrompt(Actor* Interactor)
	{
		CurrentInteractor = Interactor;
		
		OriginalFacingDirection = FacingDirection;
		
		FacePlayer();
		
		if (!bInDialog)
		{
			PromptUI.Show();
			PromptUI.SetPosition(GetPosition(), { 0.f, -100.f });
		}
	}

	void AoiMizukawa::HidePrompt(Actor* Interactor)
	{
		CurrentInteractor = nullptr;
		PromptUI.Hide();
		
		if (bInDialog)
		{
			EndDialog();
		}
		
		FacingDirection = OriginalFacingDirection;
	}

	void AoiMizukawa::StartDialog(PlayerCharacter* Player)
	{
		bInDialog = true;
		PromptUI.Hide();
		FacePlayer();

		auto& Quest = Player->GetQuest();

		if (!Quest.HasMetAoi())
		{
			DialogBox.SetDialog({
				"Hey! Didn't expect to see someone new out here.",
				"I'm Aoi. Aoi Mizukawa.",
				"Let me guess... you're looking for the Water Engine?",
				"People pass through here every now and then for it.",
				"If anyone actually knows the way, it's my grandpa, Kiyoshi.",
				"You can find him wandering around here somewhere."
				});
			Quest.MarkMetAoi();
		}
		else if (!Quest.HasFoundGrandpa())
		{
			DialogBox.SetDialog({
				"No sign of him yet?",
				"He never stays in one place long.",
				"Check the paths, he'll circle back through there eventually."
				});
		}
		else if (!Quest.HasKiyoshiItem())
		{
			DialogBox.SetDialog({
				"You found him already?",
				"Let me guess... he lost something again.",
				"It's always something with him.",
				"Whatever it is, it's probably important, he won't stop talking until it's back in his hands."
				});
		}
		else if (!Quest.CanExitForest())
		{
			DialogBox.SetDialog({
				"You found grandpa's compass? Nice.",
				"Like I said, he's always losing something.",
				"Thank you for helping him, I hope you find what you're looking for."
				});
		}
		else
		{
			DialogBox.SetDialog({
				"So, heading north now?",
				"The forest up there can be intiidating, be mindful.",
				"Good luck!"
				});
		}

		DialogBox.Show();
	}

	bool AoiMizukawa::AdvanceDialog()
	{
		return DialogBox.Advance();
	}

	void AoiMizukawa::EndDialog()
	{
		bInDialog = false;
		DialogBox.Hide();
		
		FacingDirection = OriginalFacingDirection;
	}

	void AoiMizukawa::GetDrawables(vector<const drawable*>& OutDrawables) const
	{
		if (ShadowSprite && ShadowTexture)
			OutDrawables.push_back(&ShadowSprite.value());

		Character::GetDrawables(OutDrawables);
	}

	void AoiMizukawa::SetupShadow()
	{
		ShadowTexture = LoadAsset().LoadTexture("Assets/Textures/Game/shadow.png");

		if (ShadowTexture)
		{
			ShadowSprite.emplace(*ShadowTexture);
			vec2u TexSize = ShadowTexture->getSize();
			ShadowSprite->setOrigin({ TexSize.x / 2.0f, TexSize.y / 2.0f });
			ShadowSprite->setColor({ 0, 0, 0, 150 });
			ShadowSprite->setScale({ .7,.7 });
		}
	}
}
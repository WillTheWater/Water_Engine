// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "NPC/Kiyoshi.h"
#include "Component/AnimationComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/MovementComponent.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Math.h"
#include "Player/PlayerCharacter.h"

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

	void Kiyoshi::BeginPlay()
	{
		// Kinematic body - moves autonomously
		PhysicsComp->SetBodyType(b2_kinematicBody);
		PhysicsComp->SetShapeType(PhysicsComponent::EShapeType::Circle);
		PhysicsComp->SetShapeSize({42.0f, 42.0f});
		PhysicsComp->SetLinearDamping(10.0f);

		CollComp->SetRadius(64.0f);
		CollComp->SetCollisionChannel(ECollisionChannel::Interaction);

		MoveComp->SetSpeed(120);

		Character::BeginPlay();

		SetScale({ 2.1, 2.1 });
		SetupAnimations();
		SetupShadow();

		PromptUI.Initialize("Talk");
		
		DialogBox.Initialize();
		DialogBox.SetPosition(GetPosition(), { 0.f, -200.f });
		
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
		if (AIState == EAIState::Moving && !Waypoints.empty())
		{
			vec2f TargetPos = Waypoints[CurrentWaypointIndex];
			vec2f CurrentPos = GetPosition();
			vec2f ToTarget = TargetPos - CurrentPos;
			float DistSq = ToTarget.lengthSquared();

			if (DistSq < 25.0f)
			{
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
				vec2f Direction = ToTarget / std::sqrt(DistSq);
				MoveComp->AddInputVector(Direction);
			}
		}

		UpdateDirectionalAnimation();

		Character::Tick(DeltaTime);
		if (ShadowSprite)
		{
			ShadowSprite->setPosition(GetPosition() + ShadowOffset);
		}
		
		// Update dialog position to follow Kiyoshi
		if (bInDialog)
		{
			DialogBox.SetPosition(GetPosition(), { 0.f, -200.f });
		}
	}

	void Kiyoshi::FacePlayer()
	{
		if (!CurrentInteractor)
			return;

		vec2f ToInteractor = CurrentInteractor->GetPosition() - GetPosition();
		if (LengthSquared(ToInteractor) > EPSILON)
		{
			MoveComp->SetLastMoveDirection(ToInteractor);
		}
	}

	void Kiyoshi::ShowPrompt(Actor* Interactor)
	{
		CurrentInteractor = Interactor;
		
		GetTimer().ClearTimer(WaitTimer);
		WaitTimer = TimerHandle();
		
		if (AIState != EAIState::Interacting)
		{
			AIState = EAIState::Interacting;
			MoveComp->ClearInput();
			FacePlayer();
		}
		
		if (!bInDialog)
		{
			PromptUI.Show();
			PromptUI.SetPosition(GetPosition(), { 0.f, -100.f });
		}
	}

	void Kiyoshi::HidePrompt(Actor* Interactor)
	{
		CurrentInteractor = nullptr;
		PromptUI.Hide();

		if (bInDialog)
		{
			EndDialog();
		}

		// Resume patrol
		AIState = EAIState::Waiting;
		float WaitTime = RNG().Random(1.0f, 4.0f);
		WaitTimer = GetTimer().SetTimer(
			weak_from_this(),
			&Kiyoshi::OnResumeFromInteraction,
			WaitTime,
			false
		);
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

	void Kiyoshi::SetupShadow()
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

	void Kiyoshi::GetDrawables(vector<const drawable*>& OutDrawables) const
	{
		// Add SHADOW FIRST so it renders UNDER the character
		if (ShadowSprite && ShadowTexture)
			OutDrawables.push_back(&ShadowSprite.value());

		// Then add all the normal character drawables (sprite + debug)
		Character::GetDrawables(OutDrawables);
	}

	void Kiyoshi::StartDialog(PlayerCharacter* Player)
	{
		bInDialog = true;
		PromptUI.Hide();

		FacePlayer();

		auto& Quest = Player->GetQuest();

		if (!Quest.HasMetAoi())
		{
			// Haven't spoken to Aoi yet
			DialogBox.SetDialog({
				"Hm? You look lost.",
				"Best keep to the paths. The forest is unforgiving to wanderers."
				});
		}
		else if (!Quest.HasFoundGrandpa())
		{
			// First meeting after Aoi sent you
			DialogBox.SetDialog({
				"Ah... Aoi sent you, did she?",
				"I'm Kiyoshi.",
				"You're looking for the Water Engine, I take it.",
				"Funny thing... I used to know these woods like the back of my hand.",
				"These day not so much. I had a compass around here somewhere. Maybe you can help me find it.",
				"Without it, north might as well be everywhere at once.",
				"If you happen across it, bring it back to me."
				});
			Quest.MarkFoundGrandpa();
		}
		else if (!Quest.HasKiyoshiItem())
		{
			// Still looking for the compass
			DialogBox.SetDialog({
				"No compass yet?",
				"It's a small thing, but it's kept me on track for years.",
				"Check along the paths, I wouldn't have strayed far."
				});
		}
		else if (!Quest.CanExitForest())
		{
			// Found the item - give compass and unlock path
			DialogBox.SetDialog({
				"There it is... my compass.",
				"Heh... still pointing true.",
				"You know what? You can keep it.",
				"You'll make better use of it than I will.",
				"If you're heading for the Water Engine, listen carefully.",
				"Take the northern trail into the forest.",
				"Don't stray, don't second-guess it.",
				"Follow the path, and it'll take you where you need to go."
				});
			Quest.MarkCanExit();
		}
		else
		{
			// Quest complete
			DialogBox.SetDialog({
				"Still carrying that compass?",
				"Good.",
				"Trust it more than your instincts in those woods.",
				"They'll lead you in circles."
				});
		}

		DialogBox.Show();
	}

	bool Kiyoshi::AdvanceDialog()
	{
		return DialogBox.Advance();
	}

	void Kiyoshi::EndDialog()
	{
		bInDialog = false;
		DialogBox.Hide();
	}

	void Kiyoshi::SetupAnimations()
	{
		SpriteSheet IdleSheet("Assets/Textures/Game/oldIdle.png", vec2u{ 100, 128 }, 8);
		AnimComp->AddSpriteSheet(EKiyoshiSheet::Idle, IdleSheet);
		
		SpriteSheet WalkSheet("Assets/Textures/Game/oldWalk.png", vec2u{ 100, 128 }, 8);
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
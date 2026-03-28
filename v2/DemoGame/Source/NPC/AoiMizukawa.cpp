// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "NPC/AoiMizukawa.h"
#include "Component/AnimationComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/CollisionComponent.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	enum class EAoiState : uint8
	{
		Idle
	};

	enum class EAoiSheet : uint8
	{
		Idle
	};

	AoiMizukawa::AoiMizukawa(World& OwningWorld)
		: Character(OwningWorld)
	{
	}

	AoiMizukawa::~AoiMizukawa() = default;

	void AoiMizukawa::Interact(Actor* Interactor)
	{
		LOG("[AoiMizukawa] Welcome! Please enjoy your stay.");
	}

	void AoiMizukawa::BeginPlay()
	{
		PhysicsComp->SetBodyType(b2_staticBody);
		PhysicsComp->SetShapeType(PhysicsComponent::EShapeType::Circle);
		PhysicsComp->SetShapeSize({42.0f, 42.0f});

		CollComp->SetRadius(64.0f);
		// Note: No overlap bindings - player detects us, not the other way around

		Character::BeginPlay();

		SetupAnimation();

		PromptUI.Initialize("Talk");
		PromptUI.SetPosition(GetPosition(), {0.f, -100.f});
	}

	void AoiMizukawa::Tick(float DeltaTime)
	{
		Character::Tick(DeltaTime);
	}

	void AoiMizukawa::EndPlay()
	{
		Character::EndPlay();
	}

	void AoiMizukawa::SetupAnimation()
	{
		// Sprite sheet: girlidle.png - 1 row x 8 columns, 256x256 per frame
		SpriteSheet IdleSheet("Assets/Textures/Game/girlidle.png", vec2u{256, 256}, 8);
		AnimComp->AddSpriteSheet(EAoiSheet::Idle, IdleSheet);

		// Single idle animation
		AnimComp->AddAnimation(Animation{EAoiState::Idle, EAoiSheet::Idle, vec2u{0, 0}, vec2u{0, 7}});

		// Play idle animation
		AnimComp->TransitionTo(EAoiState::Idle);
	}

	void AoiMizukawa::ShowPrompt(Actor* Interactor)
	{
		PromptUI.Show();
	}

	void AoiMizukawa::HidePrompt(Actor* Interactor)
	{
		PromptUI.Hide();
	}
}
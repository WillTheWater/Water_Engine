// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interaction/Compass.h"
#include "Component/CollisionComponent.h"
#include "Component/PhysicsComponent.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Player/PlayerCharacter.h"

namespace we
{
	Compass::Compass(World& OwningWorld)
		: Actor(OwningWorld)
	{
	}

	Compass::~Compass() = default;

	void Compass::BeginPlay()
	{
		Actor::BeginPlay();

		SetupSprite();

		// Setup interaction collision
		CollComp = make_shared<CollisionComponent>(this);
		CollComp->SetRadius(64.0f);
		CollComp->SetCollisionChannel(ECollisionChannel::Interaction);
		CollComp->BeginPlay();

		PhysicsComp = make_shared<PhysicsComponent>(this);
		PhysicsComp->SetBodyType(b2_staticBody);
		PhysicsComp->SetShapeType(PhysicsComponent::EShapeType::Circle);
		PhysicsComp->SetShapeSize({ 30, 40 });
		PhysicsComp->SetShapeOffset({0,10});
		PhysicsComp->BeginPlay();

		// Initialize prompt
		PromptUI.Initialize("Pick Up");
		PromptUI.SetPosition(GetPosition(), { 0.f, -50.f });

		SetScale({ 0.8f, 0.8f });
	}

	void Compass::EndPlay()
	{
		if (CollComp)
		{
			CollComp->EndPlay();
		}
		Actor::EndPlay();
	}

	void Compass::GetDrawables(vector<const drawable*>& OutDrawables) const
	{
		Actor::GetDrawables(OutDrawables);

		if (PhysicsComp && PhysicsComp->IsDebugDrawEnabled())
		{
			if (const auto* Debug = PhysicsComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}
	}

	void Compass::SetupSprite()
	{
		CompassTexture = LoadAsset().LoadTexture("Assets/Textures/Game/compass.png");

		if (CompassTexture)
		{
			CompassSprite.emplace(*CompassTexture);
			vec2u TexSize = CompassTexture->getSize();
			CompassSprite->setOrigin({ TexSize.x / 2.0f, TexSize.y / 2.0f });
			SetSprite(CompassTexture);
			SetSpriteOrigin({ TexSize.x / 2.0f, TexSize.y / 2.0f });
		}
	}

	void Compass::Interact(Actor* Interactor)
	{
		auto* Player = dynamic_cast<PlayerCharacter*>(Interactor);
		if (!Player) return;

		// Mark quest item found
		Player->GetQuest().MarkItemFound();

		// Hide prompt and destroy
		PromptUI.Hide();
		CollComp->EndPlay();
		Destroy();
	}

	void Compass::ShowPrompt(Actor* Interactor)
	{
		PromptUI.Show();
		PromptUI.SetPosition(GetPosition(), { 0.f, -50.f });
	}

	void Compass::HidePrompt(Actor* Interactor)
	{
		PromptUI.Hide();
	}
}
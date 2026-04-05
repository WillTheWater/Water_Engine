// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interaction/ForestExit.h"
#include "Player/PlayerCharacter.h"

namespace we
{
	ForestExit::ForestExit(World& OwningWorld)
		: Actor(OwningWorld)
	{
	}

	ForestExit::~ForestExit() = default;

	void ForestExit::BeginPlay()
	{
		Actor::BeginPlay();

		// Setup trigger collision
		CollComp = make_shared<CollisionComponent>(this);
		CollComp->SetRadius(100.0f);
		CollComp->SetCollisionChannel(ECollisionChannel::Interaction);
		CollComp->OnBeginOverlap.Bind(this, &ForestExit::OnBeginOverlap);
		CollComp->BeginPlay();
	}

	void ForestExit::EndPlay()
	{
		Actor::EndPlay();
	}

	void ForestExit::OnBeginOverlap(Actor* Other)
	{
		auto* Player = dynamic_cast<PlayerCharacter*>(Other);
		if (!Player) return;

		// Only allow exit if quest is complete
		if (Player->GetQuest().CanExitForest())
		{
			OnExitTriggered.Broadcast();
		}
	}
}
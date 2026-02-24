// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor/Character.h"
#include "Interface/Component/CameraComponent.h"
#include "Interface/Component/IMovementComponent.h"
#include "Interface/IInteractive.h"
#include "GameConfig.h"

namespace we
{
	class AnimationComponent;

	enum class NPCAnimState : uint8
	{
		None = 0,
		Idle
	};

	class NPC : public Character, public IInteractive
	{
	public:
		NPC(World* OwningWorld, const string& TexturePath = GC.NPCSheetIdle, const string& InName = "NPC", const string& InDialog = "Hello! I am an NPC.");

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Destroy() override;

	public:
		void InitializeAnimations();

		// IInteractive interface
		virtual void OnInteract(Actor* Interactor) override;
		virtual bool CanInteract() const override { return true; }
		virtual string GetInteractionPrompt() const override { return "Talk to " + Name; }

	private:
		shared<AnimationComponent> AnimComp;
		string Name;
		string Dialog;
	};
}

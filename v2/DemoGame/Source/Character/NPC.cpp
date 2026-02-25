// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/NPC.h"
#include "Interface/Component/AnimationComponent.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "UI/DialogBox.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	// ==========================================================================
	// Base NPC Implementation
	// ==========================================================================

	NPC::NPC(World* OwningWorld, const string& TexturePath, const string& InName)
		: Character(OwningWorld, TexturePath)
		, Name(InName)
		, Dialog("Hello! I am " + InName + ".")
		, DialogTitle(InName)
	{
		SetCharacterRadius(40.0f);
		SetCollisionOffset({ 0, 10 });
	}

	NPC::~NPC() = default;

	void NPC::BeginPlay()
	{
		// Derived class sets up animations
		InitializeAnimations();
		
		Character::BeginPlay();
		
		SetupInteractionSensor();
		
		// Create DialogBox UI
		DialogUI = make_unique<DialogBox>(GetWorld()->GetSubsystem(), EWidgetSpace::World);
		DialogUI->SetDialogText(Dialog);
		DialogUI->SetTitleText(DialogTitle);
		DialogUI->SetLocalOffset({ 0.f, -220.f }); // Position above NPC
	}

	void NPC::SetupInteractionSensor()
	{
		if (PhysicsComp)
		{
			// Note: Body type should be set by derived class if movement is needed
			// Default is Static for simple NPCs, Kinematic for moving NPCs
			
			// Add sensor for interaction detection (player can detect when nearby)
			PhysicsComp->SetSensorShape(true, 60.0f);  // 60 unit radius sensor
		}
	}

	void NPC::Tick(float DeltaTime)
	{
		if (AnimComp)
		{
			AnimComp->Tick(DeltaTime);
		}

		// Update DialogBox position to follow NPC
		if (DialogUI)
		{
			DialogUI->SetWorldPosition(GetPosition());
		}

		Character::Tick(DeltaTime);
	}

	void NPC::Destroy()
	{
		DialogUI.reset();
		
		if (AnimComp) AnimComp->EndPlay();
		Character::Destroy();
	}

	void NPC::OnInteract(Actor* Interactor)
	{
		LOG("[{}] Interaction triggered by: {}", Name, typeid(*Interactor).name());
		
		if (DialogUI)
		{
			if (!DialogUI->IsVisible())
			{
				ShowDialog();
			}
			else
			{
				HideDialog();
			}
		}
	}

	// ==========================================================================
	// UI Management - Delegated to DialogBox
	// ==========================================================================

	void NPC::ShowInteractionHint()
	{
		if (DialogUI)
		{
			DialogUI->ShowInteractionHint();
		}
	}

	void NPC::HideInteractionHint()
	{
		if (DialogUI)
		{
			DialogUI->HideInteractionHint();
		}
	}

	void NPC::ShowDialog()
	{
		if (DialogUI)
		{
			DialogUI->SetDialogText(Dialog);
			DialogUI->SetTitleText(DialogTitle);
			DialogUI->Show();
		}
	}

	void NPC::HideDialog()
	{
		if (DialogUI)
		{
			DialogUI->Hide();
		}
	}

	bool NPC::IsDialogVisible() const
	{
		return DialogUI && DialogUI->IsVisible();
	}

	void NPC::OnPlayerEnteredRange(Actor* Player)
	{
		(void)Player;
	}

	void NPC::OnPlayerLeftRange(Actor* Player)
	{
		HideDialog();
		(void)Player;
	}
}

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
		
		// Create DialogBox UI (delegates UI creation to DialogBox class)
		DialogUI = make_unique<DialogBox>(GetWorld()->GetSubsystem(), EWidgetSpace::World);
		DialogUI->SetDialogText(Dialog);
		DialogUI->SetTitleText(DialogTitle);
		DialogUI->SetLocalOffset({ -150.f, -120.f }); // Position above NPC
		
		LOG("[{}] DialogBox UI created", Name);
	}

	void NPC::SetupInteractionSensor()
	{
		if (PhysicsComp)
		{
			// Note: Body type should be set by derived class if movement is needed
			// Default is Static for simple NPCs, Kinematic for moving NPCs
			
			// Add sensor for interaction detection (player can detect us when nearby)
			PhysicsComp->SetSensorShape(true, 60.0f);  // 60 unit radius sensor
			
			LOG("[{}] Interaction sensor set up", Name);
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
		// Cleanup UI - DialogBox destructor handles widget cleanup
		DialogUI.reset();
		
		if (AnimComp) AnimComp->EndPlay();
		Character::Destroy();
	}

	void NPC::OnInteract(Actor* Interactor)
	{
		LOG("[{}] Interaction triggered by: {}", Name, typeid(*Interactor).name());
		
		// Toggle dialog visibility
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
			// Update text in case it changed
			DialogUI->SetDialogText(Dialog);
			DialogUI->SetTitleText(DialogTitle);
			DialogUI->Show();
			
			LOG("[{}] Showing dialog", Name);
		}
	}

	void NPC::HideDialog()
	{
		if (DialogUI)
		{
			DialogUI->Hide();
			LOG("[{}] Hiding dialog", Name);
		}
	}

	bool NPC::IsDialogVisible() const
	{
		return DialogUI && DialogUI->IsVisible();
	}

	void NPC::OnPlayerEnteredRange(Actor* Player)
	{
		// Base implementation does nothing
		// Derived classes like Kiyoshi override this
		(void)Player;
	}

	void NPC::OnPlayerLeftRange(Actor* Player)
	{
		// Base implementation - hide dialog if player leaves
		HideDialog();
		(void)Player;
	}
}

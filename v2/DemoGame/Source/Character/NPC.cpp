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
	// Static empty delegate for when DialogUI doesn't exist yet
	Delegate<> NPC::EmptyDelegate;

	// ==========================================================================
	// Base NPC Implementation
	// ==========================================================================

	NPC::NPC(World* OwningWorld, const string& TexturePath, const string& InName)
		: Character(OwningWorld, TexturePath)
		, Name(InName)
		, PendingDialogTitle(InName)
	{
		SetCharacterRadius(40.0f);
		SetCollisionOffset({ 0, 10 });
		
		// Set default single-page dialog
		PendingDialogPages.push_back("Hello! I am " + Name + ".");
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
		
		// Transfer pending dialog pages to the DialogBox
		if (!PendingDialogPages.empty())
		{
			DialogUI->SetDialogPages(PendingDialogPages);
		}
		DialogUI->SetTitleText(PendingDialogTitle.empty() ? Name : PendingDialogTitle);
		
		DialogUI->SetLocalOffset({ -160.f, -120.f }); // Center above NPC
		
		LOG("[{}] DialogBox UI created with {} pages", Name, PendingDialogPages.size());
	}

	void NPC::SetupInteractionSensor()
	{
		if (PhysicsComp)
		{
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
		
		// If dialog is already visible, close it (E key toggle behavior)
		if (DialogUI && DialogUI->IsVisible())
		{
			HideDialog();
			return;
		}
		
		// Otherwise show the dialog
		if (DialogUI)
		{
			ShowDialog();
		}
	}

	// ==========================================================================
	// Dialog Input Handling
	// ==========================================================================

	bool NPC::HandleDialogConfirm()
	{
		if (DialogUI && DialogUI->IsVisible())
		{
			// Let DialogBox handle the confirm (advance or close)
			return DialogUI->HandleConfirm();
		}
		return false;
	}

	// ==========================================================================
	// Dialog Content Methods
	// ==========================================================================

	void NPC::SetDialog(const string& InDialog)
	{
		PendingDialogPages.clear();
		PendingDialogPages.push_back(InDialog);
		
		// If DialogBox already exists, update it directly
		if (DialogUI)
		{
			DialogUI->SetDialogText(InDialog);
		}
	}

	void NPC::SetDialogPages(const vector<string>& InPages)
	{
		PendingDialogPages = InPages;
		
		// If DialogBox already exists, update it directly
		if (DialogUI)
		{
			DialogUI->SetDialogPages(InPages);
		}
	}

	void NPC::AddDialogPage(const string& InPage)
	{
		PendingDialogPages.push_back(InPage);
		
		// If DialogBox already exists, add to it directly
		if (DialogUI)
		{
			DialogUI->AddDialogPage(InPage);
		}
	}

	void NPC::ClearDialogPages()
	{
		PendingDialogPages.clear();
		
		if (DialogUI)
		{
			DialogUI->ClearDialogPages();
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
			DialogUI->Show();
			LOG("[{}] Showing dialog ({} pages)", Name, DialogUI->GetTotalPages());
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
		(void)Player;
	}

	void NPC::OnPlayerLeftRange(Actor* Player)
	{
		// When player leaves, hide dialog
		HideDialog();
		(void)Player;
	}
}

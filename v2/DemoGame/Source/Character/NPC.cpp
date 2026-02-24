// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/NPC.h"
#include "Interface/Component/AnimationComponent.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Utility/Math.h"
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
	{
		SetCharacterRadius(40.0f);
		SetCollisionOffset({ 0, 10 });
	}

	void NPC::BeginPlay()
	{
		// Derived class sets up animations
		InitializeAnimations();
		
		Character::BeginPlay();
		
		SetupInteractionSensor();
		
		// Create UI elements
		CreateInteractionHint();
		CreateDialogPanel();
	}

	void NPC::SetupInteractionSensor()
	{
		if (PhysicsComp)
		{
			PhysicsComp->SetBodyType(BodyType::Static);
			
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

		// Update UI positions to follow NPC
		UpdateUIPositions();

		Character::Tick(DeltaTime);
	}

	void NPC::Destroy()
	{
		// Hide and cleanup UI
		HideInteractionHint();
		HideDialog();
		
		if (AnimComp) AnimComp->EndPlay();
		Character::Destroy();
	}

	void NPC::OnInteract(Actor* Interactor)
	{
		LOG("[{}] Interaction triggered by: {}", Name, typeid(*Interactor).name());
		
		// Show dialog instead of just logging
		if (!bDialogVisible)
		{
			ShowDialog();
		}
		else
		{
			HideDialog();
		}
	}

	// ==========================================================================
	// UI Creation and Management
	// ==========================================================================

	void NPC::CreateInteractionHint()
	{
		auto* GUI = GetWorld()->GetSubsystem().GUI.get();
		if (!GUI) return;

		// Create "!" text block - size 40, white with black border effect
		InteractionHint = GUI->CreateTextBlock("!", 0.f, 40);
		InteractionHint->SetWidgetSpace(EWidgetSpace::World);  // Follows NPC in world space
		
		// Position above NPC head
		InteractionHint->SetLocalOffset({ 0.f, -80.f });
		
		// Hide initially
		InteractionHint->Hide();
		
		LOG("[{}] Created interaction hint", Name);
	}

	void NPC::CreateDialogPanel()
	{
		auto* GUI = GetWorld()->GetSubsystem().GUI.get();
		if (!GUI) return;

		// Create dialog panel - 240x80, black with alpha 150, 4px white border
		DialogPanel = GUI->CreatePanel(
			{ 240.f, 80.f },           // Size
			color{ 0, 0, 0, 150 },     // Black with alpha 150
			color{ 255, 255, 255, 255 }, // White border
			4.f,                        // 4px border thickness
			EWidgetSpace::World         // World space (follows NPC)
		);
		
		// Position above and to the left of NPC
		DialogPanel->SetLocalOffset({ -120.f, -120.f });
		
		// Create dialog text
		DialogText = GUI->CreateTextBlock(Dialog, 220.f, 24);
		DialogText->SetWidgetSpace(EWidgetSpace::World);
		
		// Add text to panel
		DialogPanel->AddChild(DialogText, Anchor::Center, Anchor::Center);
		
		// Hide initially
		DialogPanel->Hide();
		DialogText->Hide();
		
		LOG("[{}] Created dialog panel", Name);
	}

	void NPC::UpdateUIPositions()
	{
		if (!InteractionHint || !DialogPanel) return;

		vec2f NPCPos = GetPosition();
		
		// Update interaction hint position (above NPC head)
		InteractionHint->SetLocalOffset({ NPCPos.x, NPCPos.y - 120.f });
		
		// Update dialog panel position (above and to the left)
		DialogPanel->SetLocalOffset({ NPCPos.x - 120.f, NPCPos.y - 120.f });
	}

	void NPC::ShowInteractionHint()
	{
		if (InteractionHint && !bDialogVisible)  // Don't show if dialog is open
		{
			InteractionHint->Show();
		}
	}

	void NPC::HideInteractionHint()
	{
		if (InteractionHint)
		{
			InteractionHint->Hide();
		}
	}

	void NPC::ShowDialog()
	{
		if (DialogPanel && DialogText)
		{
			// Update text in case it changed
			DialogText->SetText(Dialog);
			
			DialogPanel->Show();
			DialogText->Show();
			bDialogVisible = true;
			
			// Hide interaction hint when dialog is showing
			HideInteractionHint();
			
			LOG("[{}] Showing dialog", Name);
		}
	}

	void NPC::HideDialog()
	{
		if (DialogPanel && DialogText)
		{
			DialogPanel->Hide();
			DialogText->Hide();
			bDialogVisible = false;
			
			LOG("[{}] Hiding dialog", Name);
		}
	}

	bool NPC::IsDialogVisible() const
	{
		return bDialogVisible;
	}
}

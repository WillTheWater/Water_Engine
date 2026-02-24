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
		DialogPanel->SetLocalOffset({ NPCPos.x - 240.f, NPCPos.y - 180.f });
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

	// ==========================================================================
	// Alice Implementation - Simple 1-way idle animation
	// ==========================================================================

	Alice::Alice(World* OwningWorld)
		: NPC(OwningWorld, GC.NPCSheetIdle, "Alice")
	{
		SetDialog("Hello! I'm Alice. Welcome to our village!");
	}

	void Alice::InitializeAnimations()
	{
		AnimComp = make_shared<AnimationComponent>(this);
		AnimComp->BeginPlay();

		// Load animation sheet (girlidle format - 256x256 frames, 8 per row)
		AnimComp->AddSpriteSheet(0, SpriteSheet{ GC.NPCSheetIdle, {256, 256}, 8 });

		// Single idle animation facing down (row 0)
		AnimComp->AddAnimation({ 1, 0, {0, 0}, {0, 7}, 0.15f, true });

		// Start with idle
		AnimComp->Transition(1);
		
		LOG("[Alice] Initialized 1-way idle animation");
	}

	// ==========================================================================
	// Bob Implementation - 8-way idle animation (old sprite sheet format)
	// ==========================================================================

	Bob::Bob(World* OwningWorld)
		: NPC(OwningWorld, GC.OldNPCSheetIdle, "Bob")
	{
		SetDialog("Greetings! I'm Bob. I sell potions and items.");
	}

	void Bob::InitializeAnimations()
	{
		AnimComp = make_shared<AnimationComponent>(this);
		AnimComp->BeginPlay();

		// Use the oldIdle sprite sheet (8x8 grid, 256x256 frames)
		AnimComp->AddSpriteSheet(0, SpriteSheet{ GC.OldNPCSheetIdle, {256, 256}, 8 });

		// IDLE animations for all 8 directions
		// Row 0: Idle Down
		AnimComp->AddAnimation({ 1, 0, {0, 0}, {0, 7}, 0.15f, true });
		// Row 1: Idle DownRight
		AnimComp->AddAnimation({ 2, 0, {1, 0}, {1, 7}, 0.15f, true });
		// Row 2: Idle Right
		AnimComp->AddAnimation({ 3, 0, {2, 0}, {2, 7}, 0.15f, true });
		// Row 3: Idle UpRight
		AnimComp->AddAnimation({ 4, 0, {3, 0}, {3, 7}, 0.15f, true });
		// Row 4: Idle Up
		AnimComp->AddAnimation({ 5, 0, {4, 0}, {4, 7}, 0.15f, true });
		// Row 5: Idle UpLeft
		AnimComp->AddAnimation({ 6, 0, {5, 0}, {5, 7}, 0.15f, true });
		// Row 6: Idle Left
		AnimComp->AddAnimation({ 7, 0, {6, 0}, {6, 7}, 0.15f, true });
		// Row 7: Idle DownLeft
		AnimComp->AddAnimation({ 8, 0, {7, 0}, {7, 7}, 0.15f, true });

		// Start facing down
		AnimComp->Transition(1);
		
		LOG("[Bob] Initialized 8-way idle animations");
	}
}

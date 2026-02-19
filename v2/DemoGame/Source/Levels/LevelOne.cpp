// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/World/Actor/Border.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/GameStateSubsystem.h"
#include "Subsystem/TimeSubsystem.h"
#include "GameInstance/DemoGameInstance.h"
#include "Input/InputActions.h"
#include "GameStateTokens.h"
#include "Character/PlayerCharacter.h"
#include "Interface/Component/PhysicsComponent.h"
#include "UI/PauseUI.h"
#include "Utility/Log.h"
#include "GameConfig.h"
#include "EngineConfig.h"

namespace we
{
	// =========================================================================
	// Trigger Test Actor - Detects overlaps but doesn't block movement
	// Uses Box2D sensor fixture
	// =========================================================================
	class TriggerTestActor : public Actor
	{
	public:
		TriggerTestActor(World* OwningWorld, const vec2f& Position, float InRadius)
			: Actor(OwningWorld, EC.WindowIcon)
			, Radius{ InRadius }
		{
			SetPosition(Position);
			SetScale({ 0.5f, 0.5f });
		}

		virtual void BeginPlay() override
		{
			PhysicsComp = make_shared<PhysicsComponent>(this);
			PhysicsComp->SetBodyType(BodyType::Static);
			PhysicsComp->SetCircleShape(Radius);
			PhysicsComp->SetSensor(true);  // Sensor = no blocking, just overlap detection
			PhysicsComp->BeginPlay();
			Actor::BeginPlay();
		}

		virtual void Tick(float DeltaTime) override
		{
			if (PhysicsComp) PhysicsComp->Tick(DeltaTime);
			Actor::Tick(DeltaTime);
		}

		virtual void Destroy() override
		{
			if (PhysicsComp) PhysicsComp->EndPlay();
			Actor::Destroy();
		}

	protected:
		shared<PhysicsComponent> PhysicsComp;
		float Radius = 60.0f;
	};

	// =========================================================================
	// Blocking Test Actor - Circle that blocks movement
	// Uses Box2D static body
	// =========================================================================
	class BlockingTestActor : public Actor
	{
	public:
		BlockingTestActor(World* OwningWorld, const vec2f& Position, float InRadius)
			: Actor(OwningWorld, EC.WindowIcon)
			, Radius{ InRadius }
		{
			SetPosition(Position);
			SetScale({ 0.8f, 0.8f });
		}

		virtual void BeginPlay() override
		{
			PhysicsComp = make_shared<PhysicsComponent>(this);
			PhysicsComp->SetBodyType(BodyType::Static);
			PhysicsComp->SetCircleShape(Radius);
			PhysicsComp->SetFriction(0.5f);
			PhysicsComp->BeginPlay();
			Actor::BeginPlay();
		}

		virtual void Tick(float DeltaTime) override
		{
			if (PhysicsComp) PhysicsComp->Tick(DeltaTime);
			Actor::Tick(DeltaTime);
		}

		virtual void Destroy() override
		{
			if (PhysicsComp) PhysicsComp->EndPlay();
			Actor::Destroy();
		}

	protected:
		shared<PhysicsComponent> PhysicsComp;
		float Radius = 70.0f;
	};

	// =========================================================================
	// Rectangle Blocking Actor - Box that blocks movement
	// Uses Box2D static body with box shape
	// =========================================================================
	class RectangleBlockingActor : public Actor
	{
	public:
		RectangleBlockingActor(World* OwningWorld, const vec2f& Position)
			: Actor(OwningWorld, EC.WindowIcon)
		{
			SetPosition(Position);
			SetScale({ 0.8f, 0.8f });
		}

		virtual void BeginPlay() override
		{
			PhysicsComp = make_shared<PhysicsComponent>(this);
			PhysicsComp->SetBodyType(BodyType::Static);
			PhysicsComp->SetBoxShape({ 90.0f, 40.0f });
			PhysicsComp->SetFriction(0.5f);
			PhysicsComp->BeginPlay();
			Actor::BeginPlay();
		}

		virtual void Tick(float DeltaTime) override
		{
			if (PhysicsComp) PhysicsComp->Tick(DeltaTime);
			Actor::Tick(DeltaTime);
		}

		virtual void Destroy() override
		{
			if (PhysicsComp) PhysicsComp->EndPlay();
			Actor::Destroy();
		}

	protected:
		shared<PhysicsComponent> PhysicsComp;
	};

	LevelOne::LevelOne(EngineSubsystem& InSubsystem)
		: World(InSubsystem)
	{
		Construct();
	}

	LevelOne::~LevelOne() = default;

	void LevelOne::Construct()
	{
		BgTexture = LoadAsset().LoadTextureSync(GC.LevelOneBackground);
		if (BgTexture)
		{
			Background.emplace(*BgTexture);
			auto bounds = Background->getLocalBounds();
			Background->setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
			Background->setPosition(vec2f(EC.RenderResolution) / 2.0f);
			AddRenderDepth(&*Background, -1000.0f);
		}

		// Create pause menu UI (hidden by default)
		PauseMenuUI = make_unique<PauseUI>(Subsystem);
		PauseMenuUI->OnContinueClicked.Bind(this, &LevelOne::OnPauseContinue);
		PauseMenuUI->OnSettingsClicked.Bind(this, &LevelOne::OnPauseSettings);
		PauseMenuUI->OnQuitClicked.Bind(this, &LevelOne::OnPauseQuit);
	}

	void LevelOne::BeginPlay()
	{
		// Spawn player at center - player spawns its own camera
		auto PlayerRef = SpawnActor<Player>();
		if (auto P = PlayerRef.lock())
		{
			P->SetPosition(EC.RenderResolution / 2.f);
		}

		SpawnActor<TriggerTestActor>(vec2f{ 200, 300 }, 60.0f);
		SpawnActor<BlockingTestActor>(vec2f{ 600, 300 }, 70.0f);
		SpawnActor<RectangleBlockingActor>(vec2f{ 1200, 400 });

		// Test Borders - Room walls (closed loop)
		auto RoomBorder = SpawnActor<Border>(vector<vec2f>{
			{ 50, 50 },
			{ 1870, 50 },
			{ 1870, 1030 },
			{ 50, 1030 }
		});
		if (auto RB = RoomBorder.lock())
		{
			RB->SetClosedLoop(true);
			RB->SetDebugColor(color{ 255, 255, 0 });  // Yellow
		}

		// Test Border - Center divider wall (open chain)
		auto DividerWall = SpawnActor<Border>(vector<vec2f>{
			{ 960, 200 },
			{ 960, 500 },
			{ 960, 800 }
		});
		if (auto DW = DividerWall.lock())
		{
			DW->SetDebugColor(color{ 0, 255, 255 });  // Cyan
		}

		// Bind input for pause toggle (ESC and Gamepad Start)
		SetupInputBindings();

		World::BeginPlay();
		Subsystem.Cursor->SetVisibility(false);
	}

	void LevelOne::SetupInputBindings()
	{
		// Bind ESC key to pause toggle
		Subsystem.Input->Bind(ACTION_TOGGLE_PAUSE, Input::Keyboard{ sf::Keyboard::Scan::Escape });

		// Bind Gamepad Start button to pause toggle
		Subsystem.Input->Bind(ACTION_TOGGLE_PAUSE, Input::Gamepad{ GamepadButton::Start, 0 });

		// Register the callback - OnPressed fires even when game is "paused" (time scale = 0)
		Subsystem.Input->OnPressed(ACTION_TOGGLE_PAUSE, [this]()
		{
			// Don't toggle if settings menu is open - let settings handle its own close
			if (bSettingsOpen)
			{
				return;
			}
			OnTogglePause();
		});
	}

	void LevelOne::OnTogglePause()
	{
		if (bPauseMenuOpen)
		{
			HidePauseMenu();
		}
		else
		{
			ShowPauseMenu();
		}
	}

	void LevelOne::ShowPauseMenu()
	{
		bPauseMenuOpen = true;
		PauseMenuUI->Show();
		
		// Show cursor when pause menu is open
		Subsystem.Cursor->SetVisibility(true);
		
		// Pause the game (freeze gameplay)
		if (Subsystem.Time)
		{
			Subsystem.Time->SetPaused(true);
		}
		

	}

	void LevelOne::HidePauseMenu()
	{
		bPauseMenuOpen = false;
		PauseMenuUI->Hide();
		
		// Hide cursor when returning to game
		Subsystem.Cursor->SetVisibility(false);
		
		// Resume the game
		if (Subsystem.Time)
		{
			Subsystem.Time->SetPaused(false);
		}
		

	}

	void LevelOne::OnPauseContinue()
	{
		// Continue just hides the menu (already handled in HidePauseMenu)
		HidePauseMenu();
	}

	void LevelOne::OnPauseSettings()
	{
		bSettingsOpen = true;
		
		// Hide pause menu temporarily while settings is open
		PauseMenuUI->Hide();
		
		// Show cursor for settings menu
		Subsystem.Cursor->SetVisibility(true);
		
		// Show settings via GameInstance (persists across worlds)
		if (auto* GI = static_cast<DemoGameInstance*>(Subsystem.GameInst.get()))
		{
			// Clear any stale bindings and bind fresh
			GI->OnSettingsClosed.Clear();
			GI->OnSettingsClosed.Bind(this, &LevelOne::OnSettingsClosed);
			GI->ShowSettings();
		}
	}

	void LevelOne::OnSettingsClosed()
	{
		bSettingsOpen = false;
		
		// If we're still paused, show cursor for pause menu
		if (bPauseMenuOpen)
		{
			Subsystem.Cursor->SetVisibility(true);
			PauseMenuUI->Show();
		}
		else
		{
			// Resume game - hide cursor
			Subsystem.Cursor->SetVisibility(false);
		}
	}

	void LevelOne::OnPauseQuit()
	{
		HidePauseMenu();
		
		// Quit the game
		Subsystem.GameState->RequestShutdown();
	}

	void LevelOne::Tick(float DeltaTime)
	{
		World::Tick(DeltaTime);
	}
}

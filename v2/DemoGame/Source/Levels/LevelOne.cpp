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
#include "Framework/World/Actor/Camera.h"
#include "Subsystem/TimeSubsystem.h"
#include "GameInstance/DemoGameInstance.h"
#include "Input/InputActions.h"
#include "GameStateTokens.h"
#include "Character/PlayerCharacter.h"
#include "Character/NPC.h"
#include "Character/Aoi.h"
#include "Character/Kiyoshi.h"
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
			Background->setPosition({ 0.f, 0.f });
			AddRenderDepth(&*Background, -100000.0f);
		}

		// Create pause menu UI (hidden by default)
		PauseMenuUI = make_unique<PauseUI>(Subsystem);
		PauseMenuUI->OnContinueClicked.Bind(this, &LevelOne::OnPauseContinue);
		PauseMenuUI->OnSettingsClicked.Bind(this, &LevelOne::OnPauseSettings);
		PauseMenuUI->OnQuitClicked.Bind(this, &LevelOne::OnPauseQuit);

		// Initialize Red X at WORLD ORIGIN (0,0)
		OriginMarker = sf::VertexArray(sf::PrimitiveType::Lines, 4);
		float size = 50.f;
		OriginMarker[0].position = { -size, -size };
		OriginMarker[1].position = {  size,  size };
		OriginMarker[2].position = { -size,  size };
		OriginMarker[3].position = {  size, -size };

		for (int i = 0; i < 4; ++i) OriginMarker[i].color = sf::Color::Red;

		// Add to render depth so it shows on top of the background
		AddRenderDepth(&OriginMarker, 999.f);
	}

	void LevelOne::BeginPlay()
	{
		PlayerRef = SpawnActor<Player>();
		if (auto P = PlayerRef.lock())
		{
			// Place player at world origin
			P->SetPosition({ 0.f, 0.f });

			auto SpawnedCam = SpawnActor<Camera>();
			if (auto C = SpawnedCam.lock())
			{
				// 1. Attach the camera to the player actor
				C->SetPosition(P->GetPosition());
				C->AttachTo(P.get(), { 0.f, 0.f });

				// 2. Set the "Lag" to 0 for a hard lock (character stays perfectly still in center)
				C->SetSmoothTime(0.3f);

				// 3. Register this as the LIVE camera in the subsystem
				if (auto* CamSys = Subsystem.Camera.get())
				{
					CamSys->SetActiveCamera(C.get());
				}

				CameraRef = SpawnedCam;
			}
		}

		// Spawn Aoi
		AoiRef = SpawnActor<Aoi>();
		if (auto aoi = AoiRef.lock())
		{
			aoi->SetPosition({ 500, -500 });
		}

		// Spawn Kiyoshi
		KiyoshiRef = SpawnActor<Kiyoshi>();
		if (auto kiyoshi = KiyoshiRef.lock())
		{
			kiyoshi->SetPosition({ -500, -500 });
		}

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
			RB->SetDebugColor(color{ 255, 255, 0 });
		}

		// Test Border - Center divider wall (open chain)
		auto DividerWall = SpawnActor<Border>(vector<vec2f>{
			{ 960, 200 },
			{ 960, 500 },
			{ 960, 800 }
		});
		if (auto DW = DividerWall.lock())
		{
			DW->SetDebugColor(color{ 0, 255, 255 });
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

		// Bind interaction inputs
		Subsystem.Input->Bind(ACTION_INTERACT, Input::Keyboard{ sf::Keyboard::Scan::E });
		Subsystem.Input->Bind(ACTION_INTERACT, Input::Gamepad{ GamepadButton::South, 0 });

		// Register interaction callback
		Subsystem.Input->OnPressed(ACTION_INTERACT, [this]()
		{
			if (Subsystem.Time->IsPaused())
			{
				return;
			}

			if (auto Player = PlayerRef.lock())
			{
				Player->TryInteract();
			}
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
		Subsystem.Cursor->SetVisibility(true);
		
		if (Subsystem.Time)
		{
			Subsystem.Time->SetPaused(true);
		}
		

	}

	void LevelOne::HidePauseMenu()
	{
		bPauseMenuOpen = false;
		PauseMenuUI->Hide();
		Subsystem.Cursor->SetVisibility(false);
		
		if (Subsystem.Time)
		{
			Subsystem.Time->SetPaused(false);
		}
	}

	void LevelOne::OnPauseContinue()
	{
		HidePauseMenu();
	}

	void LevelOne::OnPauseSettings()
	{
		bSettingsOpen = true;
		PauseMenuUI->Hide();
		Subsystem.Cursor->SetVisibility(true);
		
		if (auto* GI = static_cast<DemoGameInstance*>(Subsystem.GameInst.get()))
		{
			GI->OnSettingsClosed.Clear();
			GI->OnSettingsClosed.Bind(this, &LevelOne::OnSettingsClosed);
			GI->ShowSettings();
		}
	}

	void LevelOne::OnSettingsClosed()
	{
		bSettingsOpen = false;
		
		if (bPauseMenuOpen)
		{
			Subsystem.Cursor->SetVisibility(true);
			PauseMenuUI->Show();
		}
		else
		{
			Subsystem.Cursor->SetVisibility(false);
		}
	}

	void LevelOne::OnPauseQuit()
	{
		HidePauseMenu();
		Subsystem.GameState->RequestShutdown();
	}

	void LevelOne::Tick(float DeltaTime)
	{
		World::Tick(DeltaTime);
	}
}

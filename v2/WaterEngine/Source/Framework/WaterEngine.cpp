// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "Framework/Editor.h"
#include "Framework/World/World.h"
#include "AssetDirectory/PakDirectory.h"
#include "Utility/Log.h"
#include "Utility/Assert.h"
#include <exception>
#include <optional>

// ImGui-SFML
#include <imgui.h>
#include <imgui-SFML.h>

namespace we
{
	WaterEngine::WaterEngine()
	{
		PreConstruct();
	}

	WaterEngine::~WaterEngine() = default;

	void WaterEngine::PreConstruct()
	{
		// Create ResourceSubsystem first (no deps)
		MountAssetDirectory();
		
		// Load config using ResourceSubsystem
		LoadEngineConfig();
		
		// Create subsystems
		CreateSubsystems();
		
		// Create editor
		EditorInstance = make_unique<Editor>(Subsystem);
	}

	void WaterEngine::MountAssetDirectory()
	{
		// Create ResourceSubsystem first - no dependencies
		Subsystem.Resources = make_unique<ResourceSubsystem>();

#ifdef WE_RELEASE
		// Release: Mount Content.pak and pass to ResourceSubsystem
		try
		{
			auto pak = make_shared<PakDirectory>("Content.pak");
			Subsystem.Resources->SetAssetDirectory(pak);
			LOG("Mounted Content.pak");
		}
		catch (const std::exception& e)
		{
			LOG("Failed to mount Content.pak: {}", e.what());
		}
#else
		// Debug: Use loose files - no pak mounted
		LOG("Debug mode: Using loose files (no pak mounted)");
#endif
	}

	void WaterEngine::LoadEngineConfig()
	{
		VERIFY(Subsystem.Resources && "ResourceSubsystem must exist before loading config!");
		
		bool bLoaded = false;
		const char* ConfigPath = "Content/Config/EngineConfig.ini";

#ifdef WE_RELEASE
		// Release: Load from mounted pak via ResourceSubsystem
		if (auto assetDir = Subsystem.Resources->GetAssetDirectory())
		{
			bLoaded = EngineConfigManager::LoadFromAssetDirector(*assetDir, ConfigPath);
		}
#else
		// Debug: Load from loose file
		bLoaded = EngineConfigManager::LoadFromFile(ConfigPath);
#endif

		if (!bLoaded)
		{
			LOG("Failed to load EngineConfig.ini, using defaults");
		}
		else
		{
			LOG("EngineConfig loaded successfully");
		}
	}

	void WaterEngine::CreateSubsystems()
	{
		const EngineConfig& Config = EngineConfigManager::Get();
		
		// Create all subsystems with their config dependencies
		Subsystem.Window = make_unique<WindowSubsystem>(Config.Window);
		Subsystem.Camera = make_unique<CameraSubsystem>();
		Subsystem.Render = make_unique<RenderSubsystem>(Config.Render, *Subsystem.Window);
		Subsystem.Time = make_unique<TimeSubsystem>();
		
		// Subsystem.Worlds and Subsystem.Resources created earlier
	}

	void WaterEngine::Initialize()
	{
		VERIFY(Subsystem.Window && "Window not created!");
		VERIFY(Subsystem.Resources && "Resources not created!");
		
		// Initialize ImGui
		bool imguiInit = ImGui::SFML::Init(*Subsystem.Window);
		VERIFY(imguiInit && "ImGui::SFML::Init failed!");

		// Initialize Editor
		if (EditorInstance)
		{
			EditorInstance->Initialize();
		}

		LOG("WaterEngine Initialized");
	}

	void WaterEngine::SetMode(EngineMode NewMode)
	{
		if (CurrentMode == NewMode) return;

		if (NewMode == EngineMode::Play)
		{
			EditorInstance->OnPlay();
		}
		else
		{
			EditorInstance->OnStop();
		}

		CurrentMode = NewMode;
	}

	void WaterEngine::Tick()
	{
		Subsystem.Time->Tick();
		auto DeltaTime = Subsystem.Time->GetDeltaTime();
		Subsystem.Resources->PollCompletedRequests();

		// Update ImGui
		ImGui::SFML::Update(*Subsystem.Window, sf::seconds(DeltaTime));

		if (CurrentMode == EngineMode::Play)
		{
			if (!Subsystem.Time->IsPaused())
			{
				TickGame(DeltaTime);
			}
		}
		else
		{
			// Edit or Simulate mode
			TickEditor(DeltaTime);
		}
	}

	void WaterEngine::TickGame(float DeltaTime)
	{
		// Game project overrides this
		if (Subsystem.Worlds && Subsystem.Worlds->GetCurrentWorld())
		{
			Subsystem.Worlds->GetCurrentWorld()->TickGlobal(DeltaTime);
		}
	}

	void WaterEngine::TickEditor(float DeltaTime)
	{
		// Simulate mode: world ticks, editor UI shows
		if (EditorInstance->IsSimulating())
		{
			if (Subsystem.Worlds && Subsystem.Worlds->GetCurrentWorld())
			{
				Subsystem.Worlds->GetCurrentWorld()->TickGlobal(DeltaTime);
			}
		}
		// Edit mode: world frozen, only editor UI updates
	}

	void WaterEngine::Render()
	{
		// Begin render frame
		Subsystem.Render->BeginFrame();

		// Get camera view if available
		std::optional<CameraView> CamView;
		if (Subsystem.Camera)
		{
			CamView = Subsystem.Camera->GetCurrentView();
		}

		// Set world view (uses camera if available, default if not)
		Subsystem.Render->SetWorldView(CamView);

		// TODO: Render world actors, UI, etc. via Subsystem.Render->Draw()

		// End frame (display targets)
		Subsystem.Render->EndFrame();

		// Clear window
		Subsystem.Window->clear(color::Black);

		if (CurrentMode == EngineMode::Play)
		{
			// Full game rendering - get composited output with letterboxing
			sprite FinalFrame = Subsystem.Render->FinishComposite();
			Subsystem.Window->draw(FinalFrame);
			
			// Draw cursor on top (1:1 with window)
			Subsystem.Render->PresentCursor();
		}
		else
		{
			// Editor mode: Editor UI shows world texture via GetWorldTexture()
			EditorInstance->DrawUI();
		}

		// Render ImGui
		ImGui::SFML::Render(*Subsystem.Window);

		Subsystem.Window->display();
	}

	void WaterEngine::ProcessEvents()
	{
		while (const auto Event = Subsystem.Window->pollEvent())
		{
			// Editor gets first dibs (except in Play mode)
			if (CurrentMode != EngineMode::Play)
			{
				if (EditorInstance->ProcessEvent(*Event))
					continue;
			}

			// Window close
			if (Event->is<sf::Event::Closed>())
			{
				Subsystem.Window->close();
			}
		}
	}

	bool WaterEngine::IsRunning() const
	{
		return Subsystem.Window->isOpen();
	}
}

// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "AssetDirectory/PakDirectory.h"
#include "Utility/Log.h"
#include <exception>

namespace we
{
	WaterEngine::WaterEngine()
	{
		PreConstruct();
	}

	WaterEngine::~WaterEngine() = default;

	void WaterEngine::PreConstruct()
	{
		MountAssetDirectory();
		CreateSubsystems();
	}

	void WaterEngine::MountAssetDirectory()
	{
#ifdef WE_RELEASE
		// Release: Mount Content.pak file
		try
		{
			Subsystem.AssetDirectory = make_unique<PakDirectory>("Content.pak");
			LOG("Mounted Content.pak");
		}
		catch (const std::exception& e)
		{
			LOG("Failed to mount Content.pak: {}", e.what());
			// Fall back to loose files if pak fails
			Subsystem.AssetDirectory = nullptr;
		}
#else
		// Debug: Use loose files - AssetDirectory stays nullptr
		// Files are accessed directly from filesystem
		LOG("Debug mode: Using loose files (no pak mounted)");
#endif
	}

	void WaterEngine::CreateSubsystems()
	{
		Subsystem.Window = make_unique<WindowSubsystem>();
	}

	void WaterEngine::Initialize()
	{
		LoadEngineConfig();
		ApplyEngineConfig();

		LOG("WaterEngine Initialized");
	}

	void WaterEngine::LoadEngineConfig()
	{
		bool bLoaded = false;
		const char* ConfigPath = "Content/Config/EngineConfig.ini";

#ifdef WE_RELEASE
		// Release: Load from mounted pak
		if (Subsystem.AssetDirectory)
		{
			bLoaded = EngineConfigManager::LoadFromAssetDirector(*Subsystem.AssetDirectory, ConfigPath);
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

	void WaterEngine::ApplyEngineConfig()
	{
		const EngineConfig& Config = EngineConfigManager::Get();

		Subsystem.Window->setTitle(Config.Window.Title);

		LOG("EngineConfig applied");
	}

	void WaterEngine::Tick()
	{
	}

	void WaterEngine::Render()
	{
		Subsystem.Window->clear(color::Black);
		Subsystem.Window->display();
	}

	void WaterEngine::ProcessEvents()
	{
		while (const auto Event = Subsystem.Window->pollEvent())
		{
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

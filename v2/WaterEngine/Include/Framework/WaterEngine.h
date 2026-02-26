// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	class WaterEngine
	{
	public:
		WaterEngine();
		virtual ~WaterEngine();

		// Main loop functions
		void Initialize();
		void Tick();
		void Render();
		void ProcessEvents();
		bool IsRunning() const;


	private:
		EngineSubsystem Subsystem;
		void PreConstruct();
		void MountAssetDirectory();
		void CreateSubsystems();
		void LoadEngineConfig();
		void ApplyEngineConfig();
	};
}

// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	class WaterEngine
	{
	public:
		WaterEngine();
		~WaterEngine();

		void Run();
		void ProcessEvents();
		void Update();
		void PostUpdate();

	public:
		bool IsRunning() const;
		bool HasFocus() const;
		void Shutdown();

	private:
		EngineSubsystem Subsystem;

	private:
		void PreConstruct();
		void MountAssetDirectory();
		void TickGame();

		// These need to be removed
		void WindowInit();
		void ConstrainRender(vec2u NewSize);
	};
}
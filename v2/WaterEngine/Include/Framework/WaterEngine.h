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

		void Initialize();
		void Tick();
		void Render();
		void PostUpdate();
		void ProcessEvents();

	public:
		bool IsRunning() const;
		bool HasFocus() const;

	protected:
		EngineSubsystem Subsystem;

	protected:
		virtual void Construct() {}
		virtual void BeginPlay() {}
		virtual void Tick(float) {}

	private:
		void PreConstruct();
		void MountAssetDirectory();
		void TickGame();
		void WorldRender();
		void Run() = delete;
	};
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

#include "Framework/GameWindow.h"
#include "Framework/EngineSubsystem.h"
#include "Input/InputEventHandler.h"

namespace we
{
	class WaterEngine
	{
	public:
		WaterEngine();

		void ProcessEvents();
		void GlobalTick();
		void PostUpdate();
		void Render();

		bool IsRunning() const;
		bool HasFocus() const;

	private:
		EngineSubsystem Subsystem;
		unique<GameWindow> Window;

	private:
		friend InputEventHandler;

		void Configure();
		void Construct();
		void WindowInit();
		void ConstrainRender();
	};
}
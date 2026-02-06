// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/GameWindow.h"

namespace we
{
	class WaterEngine
	{
	public:
		WaterEngine();
		~WaterEngine();

		void Run();
		void ProcessEvents();
		void Render();
		void PostUpdate();

		bool IsRunning() const;
		bool HasFocus() const;
		void Quit();

	public:
		virtual void Tick(float DeltaTime);

	protected:
		EngineSubsystem Subsystem;

	private:
		unique<GameWindow> Window;
		bool bExitRequested;

	private:
		void TickGame();
		void Configure();
		void Construct();
		void WindowInit();
		void ConstrainRender(vec2u NewSize);
	};
}
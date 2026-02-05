// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/GameWindow.h"
#include "UI/Cursor/Cursor.h"

namespace we
{
	class WaterEngine
	{
	public:
		WaterEngine();
		~WaterEngine();

		void ProcessEvents();
		void GlobalTick();
		void PostUpdate();
		void Render();

		bool IsRunning() const;
		bool HasFocus() const;
		void Quit();

	public:
		virtual void Tick(float DeltaTime);

	protected:
		EngineSubsystem Subsystem;

	private:
		unique<GameWindow> Window;
		bool bExitRequested = false;

	private:
		void Configure();
		void Construct();
		void WindowInit();
		void ConstrainRender();
	};
}
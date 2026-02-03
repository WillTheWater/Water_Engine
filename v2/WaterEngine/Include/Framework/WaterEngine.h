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
	class World;

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

	private:
		EngineSubsystem Subsystem;
		unique<GameWindow> Window;
		unique<Cursor> WindowCursor;

	private:
		void Configure();
		void Construct();
		void WindowInit();
		void ConstrainRender();
	};
}
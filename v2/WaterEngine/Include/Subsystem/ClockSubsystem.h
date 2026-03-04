// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	class ClockSubsystem
	{
	public:
		float GetDeltaTime() const;
		float GetElapsedTime() const;

		void SetPaused(bool bInPaused);
		void Pause() { SetPaused(true); }
		void Resume() { SetPaused(false); }
		bool IsPaused() const { return bPaused; }

	private:
		clock GlobalTick;
		time DeltaTime;
		time PreviousTick;
		bool bPaused = false;

	private:
		friend class WaterEngine;
		void Tick();
	};
}
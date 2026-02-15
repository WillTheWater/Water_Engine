// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	class TimeSubsystem
	{
	public:
		float GetDeltaTime() const;
		float GetUnscaledDeltaTime() const;
		float GetElapsedTime() const;

		void SetPaused(bool bInPaused);
		bool IsPaused() const { return bPaused; }

		void SetTimeScale(float Scale);
		float GetTimeScale() const { return TimeScale; }

	private:
		sf::Clock GlobalTick;
		sf::Time DeltaTime;
		sf::Time UnscaledDeltaTime;
		sf::Time PreviousTick;
		float TimeScale = 1.0f;
		bool bPaused = false;

	private:
		friend class WaterEngine;
		void Tick();
	};
}
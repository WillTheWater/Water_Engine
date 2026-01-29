// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

#include <sfml/System/Clock.hpp>
#include <sfml/System/Time.hpp>

namespace we
{
	class TimerSubsystem
	{
	public:
		float GetDeltaTime() const;
		float GetElapsedTime() const;

	private:
		sf::Clock GlobalTick;
		sf::Time DeltaTime;
		sf::Time PreviousTick;

	private:
		friend class WaterEngine;

		void Tick();
	};
}
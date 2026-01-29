// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <algorithm>

#include "Subsystem/TimerSubsystem.h"
#include "EngineConfig.h"

namespace we
{
	void TimerSubsystem::Tick()
	{
		const sf::Time CurrentTime = GlobalTick.getElapsedTime();
		DeltaTime = std::min(CurrentTime - PreviousTick, EC.MaxDeltaTime);
		PreviousTick = CurrentTime;
	}

	float TimerSubsystem::GetDeltaTime() const
	{
		return DeltaTime.asSeconds();
	}

	float TimerSubsystem::GetElapsedTime() const
	{
		return GlobalTick.getElapsedTime().asSeconds();
	}
}
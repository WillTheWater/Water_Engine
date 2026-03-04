// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/ClockSubsystem.h"

namespace we
{
	void ClockSubsystem::Tick()
	{
		const time CurrentTime = GlobalTick.getElapsedTime();
		
		if (bPaused)
		{
			DeltaTime = time::Zero;
			PreviousTick = CurrentTime;
		}
		else
		{
			DeltaTime = CurrentTime - PreviousTick;
			PreviousTick = CurrentTime;
		}
	}

	float ClockSubsystem::GetDeltaTime() const
	{
		return DeltaTime.asSeconds();
	}

	float ClockSubsystem::GetElapsedTime() const
	{
		return GlobalTick.getElapsedTime().asSeconds();
	}

	void ClockSubsystem::SetPaused(bool bInPaused)
	{
		bPaused = bInPaused;
	}
}

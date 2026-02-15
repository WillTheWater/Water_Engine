// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/TimeSubsystem.h"
#include "EngineConfig.h"

namespace we
{
	void TimeSubsystem::Tick()
	{
		const sf::Time CurrentTime = GlobalTick.getElapsedTime();
		UnscaledDeltaTime = std::min(CurrentTime - PreviousTick, EC.MaxDeltaTime);
		PreviousTick = CurrentTime;

		if (bPaused)
		{
			DeltaTime = sf::Time::Zero;
		}
		else
		{
			DeltaTime = sf::seconds(UnscaledDeltaTime.asSeconds() * TimeScale);
		}
	}

	float TimeSubsystem::GetDeltaTime() const
	{
		return DeltaTime.asSeconds();
	}

	float TimeSubsystem::GetUnscaledDeltaTime() const
	{
		return UnscaledDeltaTime.asSeconds();
	}

	float TimeSubsystem::GetElapsedTime() const
	{
		return GlobalTick.getElapsedTime().asSeconds();
	}

	void TimeSubsystem::SetPaused(bool bInPaused)
	{
		bPaused = bInPaused;
	}

	void TimeSubsystem::SetTimeScale(float Scale)
	{
		TimeScale = std::max(0.0f, Scale);
	}
}
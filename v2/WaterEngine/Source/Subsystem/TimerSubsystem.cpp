// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/TimerSubsystem.h"
#include "EngineConfig.h"

namespace we
{
	void TimerSubsystem::Tick()
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

	float TimerSubsystem::GetDeltaTime() const
	{
		return DeltaTime.asSeconds();
	}

	float TimerSubsystem::GetUnscaledDeltaTime() const
	{
		return UnscaledDeltaTime.asSeconds();
	}

	float TimerSubsystem::GetElapsedTime() const
	{
		return GlobalTick.getElapsedTime().asSeconds();
	}

	void TimerSubsystem::SetPaused(bool bInPaused)
	{
		bPaused = bInPaused;
	}

	void TimerSubsystem::SetTimeScale(float Scale)
	{
		TimeScale = std::max(0.0f, Scale);
	}
}
#include "Framework/TimerManager.h"

namespace we
{
	Timer::Timer(weak<Object> ObjectRef, std::function<void()> Callback, float Duration, bool Loop)
		: Listener{ObjectRef, Callback}
		, Duration{Duration}
		, bLoop{Loop}
		, TimerCounter{0.f}
		, bIsExpired{false}
	{
	}

	void Timer::TickTimer(float DeltaTime)
	{
		if (Expired()) { return; }
		TimerCounter += DeltaTime;
		if (TimerCounter >= Duration)
		{
			Listener.second();
			if (bLoop)
			{
				TimerCounter = 0.f;
			}
			else
			{
				SetExpired();
			}
		}
	}

	bool Timer::Expired() const
	{

		return bIsExpired || Listener.first.expired() || Listener.first.lock()->IsPendingDestroy();
	}

	void Timer::SetExpired()
	{
		bIsExpired = true;
	}

	unsigned int TimerManager::TimerIndex = 0;
	unique<TimerManager> TimerManager::TimerMgr{ nullptr };
	
	TimerManager::TimerManager()
		: Timers{}
	{
	}

	void TimerManager::UpdateTimer(float DeltaTime)
	{
		for (auto& Timer = Timers.begin(); Timer != Timers.end();)
		{
			if (Timer->second.Expired())
			{
				Timer = Timers.erase(Timer);
			}
			else
			{
				Timer->second.TickTimer(DeltaTime);
				Timer++;
			}
		}
	}

	void TimerManager::ClearTimer(unsigned int TimerIndex)
	{
		auto TimerHash = Timers.find(TimerIndex);
		if (TimerHash != Timers.end())
		{
			TimerHash->second.SetExpired();
		}
	}


	TimerManager& TimerManager::Get()
	{
		if (!TimerMgr)
		{
			TimerMgr = std::move(unique<TimerManager>(new TimerManager{}));
		}
		return *TimerMgr;
	}

}
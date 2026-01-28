#pragma once
#include "Framework/Core.h"
#include "Framework/Object.h"

namespace we
{
	struct TimerHandle
	{
	public:
		TimerHandle();

		unsigned int GetTimerKey() const { return TimerKey; }

	private:
		unsigned int TimerKey;
		static unsigned int TimerKeyIndex;
		static unsigned int GetNextTimerKey() { return TimerKeyIndex++; }
	};

	struct TimerHashFunction
	{
	public:
		std::size_t operator()(const TimerHandle& Handle) const
		{
			return Handle.GetTimerKey();
		}
	};

	bool operator==(const TimerHandle& lhs, const TimerHandle& rhs);

	struct Timer
	{
	public:
		Timer(weak<Object> ObjectRef, std::function<void()> Callback, float Duration, bool Loop);
		void TickTimer(float DeltaTime);
		bool Expired() const;
		void SetExpired();

	private:
		std::pair<weak<Object>, std::function<void()>> Listener;
		float Duration;
		bool bLoop;
		float TimerCounter;
		bool bIsExpired;
	};

	class TimerManager
	{
	public:
		static TimerManager& Get();

		template<typename ClassName>
		TimerHandle SetTimer(weak<Object> ObjectRef, void(ClassName::* Callback)(), float Duration, bool Loop = false)
		{
			TimerHandle NewHandle{};
			Timers.insert({ NewHandle, Timer(ObjectRef, [=] {(static_cast<ClassName*>(ObjectRef.lock().get())->*Callback)(); }, Duration, Loop) });
			return NewHandle;
		}

		void UpdateTimer(float DeltaTime);
		void ClearTimer(TimerHandle Handle);

	protected:
		TimerManager();


	private:
		static unique<TimerManager> TimerMgr;
		Dictionary<TimerHandle, Timer, TimerHashFunction> Timers;
	};
}
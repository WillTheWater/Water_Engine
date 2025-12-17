#include "Framework/Core.h"
#include "Framework/Object.h"

namespace we
{
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
		unsigned int SetTimer(weak<Object> ObjectRef, void(ClassName::* Callback)(), float Duration, bool Loop = false)
		{
			TimerIndex++;
			Timers.insert({ TimerIndex, Timer(ObjectRef, [=] {(static_cast<ClassName*>(ObjectRef.lock().get())->*Callback)(); }, Duration, Loop) });
			return TimerIndex;
		}

		void UpdateTimer(float DeltaTime);
		void ClearTimer(unsigned int TimerIndex);

	protected:
		TimerManager();


	private:
		static unique<TimerManager> TimerMgr;
		static unsigned int TimerIndex;
		Dictionary<unsigned int, Timer> Timers;		
	};
}
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
		void SetTimer(weak<Object> ObjectRef, void(ClassName::* Callback)(), float Duration, bool Loop = false)
		{
			Timers.push_back(Timer(ObjectRef, [=] {(static_cast<ClassName*>(ObjectRef.lock().get())->*Callback)();}, Duration, Loop));
		}

		void UpdateTimer(float DeltaTime);

	protected:
		TimerManager();


	private:
		static unique<TimerManager> TimerMgr;
		List<Timer> Timers;		
	};
}
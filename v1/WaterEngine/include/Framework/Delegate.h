#pragma once
#include "Framework/Core.h"

namespace we
{
	class Object;

	template<typename... Args>
	class Delegate
	{
	public:
		template<typename ClassName>
		void Bind(weak<Object> Object, void(ClassName::* Callback)(Args...))
		{
			std::function<bool(Args...)> CallbackFunc = [Object, Callback](Args... args)->bool
			{
				if (!Object.expired())
				{
					(static_cast<ClassName*>(Object.lock().get())->*Callback)(args...);
					return true;
				}
				return false;
			};
			DelegateCallbacks.push_back(CallbackFunc);
		}

		void Broadcast(Args... args)
		{
			for (auto i = DelegateCallbacks.begin(); i != DelegateCallbacks.end();)
			{
				if ((*i)(args...))
				{
					++i;
				}
				else
				{
					i = DelegateCallbacks.erase(i);
				}
			}
		}

	private:
		List<std::function<bool(Args...)>> DelegateCallbacks;
	};
}
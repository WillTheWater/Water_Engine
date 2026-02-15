// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	template<typename... Args>
	class Delegate
	{
	public:
		using CallbackFunction = std::function<bool(Args...)>;

		template<typename ClassType>
		void Bind(ClassType* Target, void(ClassType::* Method)(Args...))
		{
			Callbacks.push_back([Target, Method](Args... args) -> bool
				{
					if (Target)
					{
						(Target->*Method)(args...);
						return true;
					}
					return false;
				});
		}

		template<typename ClassType>
		void Bind(std::weak_ptr<ClassType> Target, void(ClassType::* Method)(Args...))
		{
			Callbacks.push_back([Target, Method](Args... args) -> bool
				{
					if (auto Shared = Target.lock())
					{
						(Shared.get()->*Method)(args...);
						return true;
					}
					return false;
				});
		}

		template<typename Callable>
		void Bind(Callable Func)
		{
			Callbacks.push_back([Func](Args... args) -> bool
				{
					Func(args...);
					return true;
				});
		}

		void Broadcast(Args... args)
		{
			for (auto it = Callbacks.begin(); it != Callbacks.end();)
			{
				if ((*it)(args...))
				{
					++it;
				}
				else
				{
					it = Callbacks.erase(it);
				}
			}
		}

		void Clear() { Callbacks.clear(); }
		bool IsEmpty() const { return Callbacks.empty(); }

	private:
		vector<CallbackFunction> Callbacks;
	};
}
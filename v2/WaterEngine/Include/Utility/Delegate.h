// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	// Lightweight multicast delegate system
	// Usage:
	//   Delegate<float> OnHealthChanged;
	//   OnHealthChanged.Bind(this, &Player::HandleHealthChange);
	//   OnHealthChanged.Broadcast(50.0f);
	template<typename... Args>
	class Delegate
	{
	public:
		using CallbackFunction = std::function<bool(Args...)>;

		// Bind a member function (raw pointer)
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

		// Bind a member function (weak_ptr - auto-unbinds when object destroyed)
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

		// Bind a lambda or free function
		template<typename Callable>
		void Bind(Callable Func)
		{
			Callbacks.push_back([Func](Args... args) -> bool
				{
					Func(args...);
					return true;
				});
		}

		// Alternative syntax: operator+= for lambdas
		template<typename Callable>
		Delegate& operator+=(Callable Func)
		{
			Bind(std::move(Func));
			return *this;
		}

		// Invoke all bound callbacks (stale weak_ptrs auto-remove)
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

		// Invoke operator (alternative to Broadcast)
		void operator()(Args... args) { Broadcast(std::forward<Args>(args)...); }

		void Clear() { Callbacks.clear(); }
		bool IsEmpty() const { return Callbacks.empty(); }
		uint GetBindingCount() const { return static_cast<uint>(Callbacks.size()); }

	private:
		vector<CallbackFunction> Callbacks;
	};
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/InputSubsystem.h"
#include "EventHandler/InputEventHandler.h"

namespace we
{
	InputSubsystem* InputSubsystem::Instance = nullptr;

	InputSubsystem::InputSubsystem()
	{
		Instance = this;
	}

	InputSubsystem& InputSubsystem::Get()
	{
		return *Instance;
	}

	void InputSubsystem::HandleEvent(const sf::Event& event)
	{
		InputEventHandler Handler{ *this };
		event.visit(Handler);
	}

	void InputSubsystem::Bind(int InputAction, const Input::Binding& Binding)
	{
		Bindings.emplace(InputAction, Binding);
	}

	bool InputSubsystem::Pressed(int InputAction) const
	{
		return HeldActions.contains(InputAction);
	}

	void InputSubsystem::OnPressed(const Input::Binding& Binding)
	{
		for (const auto& [Action, BoundBinding] : Bindings)
		{
			if (Input::BindingsEqual(BoundBinding, Binding))
			{
				uint& Count = ActionPressCount[Action];
				if (Count == 0)
				{
					HeldActions.insert(Action);
				}
				Count++;
			}
		}
	}

	void InputSubsystem::OnReleased(const Input::Binding& Binding)
	{
		for (const auto& [Action, BoundBinding] : Bindings)
		{
			if (Input::BindingsEqual(BoundBinding, Binding))
			{
				auto It = ActionPressCount.find(Action);
				if (It != ActionPressCount.end())
				{
					if (It->second > 0)
					{
						It->second--;
					}
					if (It->second == 0)
					{
						HeldActions.erase(Action);
					}
				}
			}
		}
	}
}
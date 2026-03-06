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

	// =============================================================================
	// STATE-BASED API
	// =============================================================================

	void InputSubsystem::Bind(int InputAction, const Input::Binding& Binding)
	{
		InputBindings.emplace(InputAction, Binding);
		ulong Hash = BindingHash{}(Binding);
		BindingToActions[Hash].push_back(InputAction);
	}

	bool InputSubsystem::IsPressed(int InputAction) const
	{
		const auto [First, Last] = InputBindings.equal_range(InputAction);
		return std::any_of(First, Last, [this](const auto& Entry) {
			return std::visit([this](const auto& Binding) {
				return this->IsPressed(Binding);
				}, Entry.second);
			});
	}

	void InputSubsystem::UnbindAll()
	{
		InputBindings.clear();
		PressedCallbacks.clear();
		ReleasedCallbacks.clear();
	}

	// =============================================================================
	// EVENT-BASED API
	// =============================================================================

	void InputSubsystem::HandleEvent(const sf::Event& event)
	{
		InputEventHandler Handler{ *this };
		event.visit(Handler);
	}

	void InputSubsystem::ProcessPressed(const Input::Binding& binding)
	{
		ulong Hash = BindingHash{}(binding);
		auto It = BindingToActions.find(Hash);
		if (It != BindingToActions.end())
		{
			for (int ActionID : It->second)
			{
				PressedOnFrame[ActionID] = CurrentFrame;
				if (auto CBIt = PressedCallbacks.find(ActionID); CBIt != PressedCallbacks.end())
				{
					for (auto& cb : CBIt->second)
					{
						cb();
					}
				}
			}
		}
	}

	void InputSubsystem::ProcessReleased(const Input::Binding& binding)
	{
		ulong Hash = BindingHash{}(binding);
		auto It = BindingToActions.find(Hash);
		if (It != BindingToActions.end())
		{
			for (int ActionID : It->second)
			{
				ReleasedOnFrame[ActionID] = CurrentFrame;
				if (auto CBIt = ReleasedCallbacks.find(ActionID); CBIt != ReleasedCallbacks.end())
				{
					for (auto& cb : CBIt->second)
					{
						cb();
					}
				}
			}
		}
	}

	void InputSubsystem::ProcessHeld()
	{
		for (const auto& [actionId, callbacks] : HeldCallbacks)
		{
			if (IsPressed(actionId))
			{
				for (auto& cb : callbacks)
				{
					cb();
				}
			}
		}
	}

	float InputSubsystem::GetAxisValue(int JoystickID, sf::Joystick::Axis Axis) const
	{
		return sf::Joystick::getAxisPosition(JoystickID, Axis) / 100.0f;
	}

	bool InputSubsystem::IsJustPressed(int InputAction) const
	{
		auto it = PressedOnFrame.find(InputAction);
		return it != PressedOnFrame.end() && it->second == CurrentFrame;
	}

	bool InputSubsystem::IsJustReleased(int InputAction) const
	{
		auto it = ReleasedOnFrame.find(InputAction);
		return it != ReleasedOnFrame.end() && it->second == CurrentFrame;
	}

	void InputSubsystem::OnPressed(int InputAction, ActionCallback Callback)
	{
		PressedCallbacks[InputAction].push_back(std::move(Callback));
	}

	void InputSubsystem::OnReleased(int InputAction, ActionCallback Callback)
	{
		ReleasedCallbacks[InputAction].push_back(std::move(Callback));
	}

	void InputSubsystem::OnHeld(int InputAction, ActionCallback Callback)
	{
		HeldCallbacks[InputAction].push_back(std::move(Callback));
	}

	void InputSubsystem::PostUpdate()
	{
		std::erase_if(PressedOnFrame, [this](const auto& pair) {
			return pair.second < CurrentFrame;
			});
		std::erase_if(ReleasedOnFrame, [this](const auto& pair) {
			return pair.second < CurrentFrame;
			});

		CurrentFrame++;
	}

	// =============================================================================
	// PRIVATE POLLING IMPLEMENTATIONS
	// =============================================================================

	bool InputSubsystem::IsPressed(const Input::Keyboard& Binding) const
	{
		return sf::Keyboard::isKeyPressed(Binding.Key);
	}

	bool InputSubsystem::IsPressed(const Input::Mouse& Binding) const
	{
		return sf::Mouse::isButtonPressed(Binding.Button);
	}

	bool InputSubsystem::IsPressed(const Input::Gamepad& Binding) const
	{
		const auto Button = Input::LogicToHardware(Binding.Button, Binding.GamepadID);
		return Button ? sf::Joystick::isButtonPressed(Binding.GamepadID, *Button) : false;
	}

	bool InputSubsystem::IsPressed(const Input::JoystickAxis& Binding) const
	{
		float AxisValue = sf::Joystick::getAxisPosition(Binding.AxisID, Binding.Axis) / 100.f;
		return (Binding.AxisThreshold >= 0) ? AxisValue > Binding.AxisThreshold : AxisValue < Binding.AxisThreshold;
	}
}
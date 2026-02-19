// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/InputSubsystem.h"
#include "Input/InputEventHandler.h"

namespace we
{
	InputSubsystem::InputSubsystem()
	{
	}

	// =============================================================================
	// STATE-BASED API
	// =============================================================================

	void InputSubsystem::Bind(int InputAction, const Input::Binding& Binding)
	{
		InputBindings.emplace(InputAction, Binding);
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
		InputEventHandler Handler{ *this};
		event.visit(Handler);
	}

	void InputSubsystem::ProcessPressed(const Input::Binding& binding)
	{
		for (const auto& [actionId, boundBinding] : InputBindings)
		{
			if (Input::BindingsEqual(binding, boundBinding))
			{
				PressedOnFrame[actionId] = CurrentFrame;

				if (auto it = PressedCallbacks.find(actionId); it != PressedCallbacks.end())
				{
					for (auto& cb : it->second) cb();
				}
			}
		}
	}

	void InputSubsystem::ProcessReleased(const Input::Binding& binding)
	{
		for (const auto& [actionId, boundBinding] : InputBindings)
		{
			if (Input::BindingsEqual(binding, boundBinding))
			{
				ReleasedOnFrame[actionId] = CurrentFrame;

				if (auto it = ReleasedCallbacks.find(actionId); it != ReleasedCallbacks.end())
				{
					for (auto& cb : it->second) cb();
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
					cb();
			}
		}
	}

	float InputSubsystem::GetAxisValue(int JoystickID, sf::Joystick::Axis Axis) const
	{
		// Poll directly on demand - no caching overhead
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
		// Poll axis directly on demand - no caching overhead
		float AxisValue = sf::Joystick::getAxisPosition(Binding.JoystickID, Binding.Axis) / 100.f;
		return (Binding.AxisThreshold >= 0) ? AxisValue > Binding.AxisThreshold : AxisValue < Binding.AxisThreshold;
	}
}
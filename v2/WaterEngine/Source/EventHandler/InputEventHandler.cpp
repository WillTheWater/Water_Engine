// =============================================================================
// Water Engine v2.1.1
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EventHandler/InputEventHandler.h"
#include "Subsystem/InputSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	void InputEventHandler::operator()(const event::JoystickConnected& Gamepad)
	{
	}

	void InputEventHandler::operator()(const event::JoystickDisconnected& Gamepad)
	{
	}

	void InputEventHandler::operator()(const event::JoystickButtonPressed& Button)
	{
		auto Logic = Input::HardwareToLogic(Button.button, Button.joystickId);
		if (Logic)
		{
			Input::Gamepad Binding{ *Logic, Button.joystickId };
			Input.OnPressed(Binding);
		}
	}

	void InputEventHandler::operator()(const event::JoystickButtonReleased& Button)
	{
		auto Logic = Input::HardwareToLogic(Button.button, Button.joystickId);
		if (Logic)
		{
			Input::Gamepad Binding{ *Logic, Button.joystickId };
			Input.OnReleased(Binding);
		}
	}

	void InputEventHandler::operator()(const event::KeyPressed& Key)
	{
		Input.OnPressed(Input::Keyboard{ Key.scancode });
	}

	void InputEventHandler::operator()(const event::KeyReleased& Key)
	{
		Input.OnReleased(Input::Keyboard{ Key.scancode });
	}

	void InputEventHandler::operator()(const event::FocusLost&)
	{
		// Clear all held keys when window loses focus
		// This prevents "stuck" keys when alt-tabbing or clicking away
		Input.ClearAllInput();
	}

	void InputEventHandler::operator()(const event::MouseMoved& Mouse)
	{

	}

	void InputEventHandler::operator()(const event::MouseButtonPressed& Button)
	{		
		Input.OnPressed(Input::Mouse{ Button.button });
	}

	void InputEventHandler::operator()(const event::MouseButtonReleased& Button)
	{
		Input.OnReleased(Input::Mouse{ Button.button });
	}

	void InputEventHandler::operator()(const event::MouseWheelScrolled& Scroll)
	{
		if (Scroll.delta == 0.0f)
			return;

		Input::Scroll Binding{ Scroll.delta > 0.0f ? ScrollDirection::Up : ScrollDirection::Down };

		int notches = std::max(1, (int)std::abs(Scroll.delta));
		for (int i = 0; i < notches; ++i)
		{
			Input.OnPressed(Binding);
			Input.OnReleased(Binding);
		}
	}
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Input/InputEventHandler.h"
#include "Subsystem/InputSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	void InputEventHandler::operator()(const sf::Event::JoystickConnected& Gamepad)
	{
		LOG("Gamepad Connected: {}", Gamepad.joystickId);
	}

	void InputEventHandler::operator()(const sf::Event::JoystickDisconnected& Gamepad)
	{
		LOG("Gamepad Disonnected: {}", Gamepad.joystickId);
	}

	void InputEventHandler::operator()(const sf::Event::JoystickButtonPressed& Button)
	{
		auto Logic = Input::HardwareToLogic(Button.button, Button.joystickId);
		if (Logic)
		{
			Input::Gamepad Binding{ *Logic, Button.joystickId };
			Input.ProcessPressed(Binding);
		}
	}

	void InputEventHandler::operator()(const sf::Event::JoystickButtonReleased& Button)
	{
		auto Logic = Input::HardwareToLogic(Button.button, Button.joystickId);
		if (Logic)
		{
			Input::Gamepad Binding{ *Logic, Button.joystickId };
			Input.ProcessReleased(Binding);
		}
	}

	void InputEventHandler::operator()(const sf::Event::KeyPressed& Key)
	{
		Input.ProcessPressed(Input::Keyboard{ Key.scancode });
	}

	void InputEventHandler::operator()(const sf::Event::KeyReleased& Key)
	{
		Input.ProcessReleased(Input::Keyboard{ Key.scancode });
	}

	void InputEventHandler::operator()(const sf::Event::MouseMoved& Mouse)
	{
		
	}

	void InputEventHandler::operator()(const sf::Event::MouseButtonPressed& Button)
	{
		Input.ProcessPressed(Input::Mouse{ Button.button });
	}

	void InputEventHandler::operator()(const sf::Event::MouseButtonReleased& Button)
	{
		Input.ProcessReleased(Input::Mouse{ Button.button });
	}
}
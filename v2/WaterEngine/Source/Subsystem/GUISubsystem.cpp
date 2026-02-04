// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GUISubsystem.h"
#include "Framework/GameWindow.h"
#include "Input/InputBinding.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"

namespace we
{
	GUISubsystem::GUISubsystem(GameWindow& Window)
		: Window{ Window }
	{
	}

	bool GUISubsystem::HandleEvents(const sf::Event& Event)
	{
		bool Handled = false;

		Event.visit([this, &Handled](const auto& Type)
		{
			this->HandleEvent(Type);
		});

		return Handled;
	}

	void GUISubsystem::Render()
	{
	}

	void GUISubsystem::HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad)
	{
		if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) == GamepadButton::South)
		{
		}
	}

	void GUISubsystem::HandleEvent(const sf::Event::JoystickButtonReleased& Gamepad)
	{
		if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) == GamepadButton::South)
		{
		}
	}
}
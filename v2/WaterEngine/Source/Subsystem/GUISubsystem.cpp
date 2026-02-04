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
		, GUI{ Window }
	{
		GUI.setTabKeyUsageEnabled(true);
		GUI.setFont(*Asset().LoadTGUIFont(EC.DefaultTitleFont));
	}

	bool GUISubsystem::HandleEvents(const sf::Event& Event)
	{
		bool Handled = false;

		Event.visit([this, &Handled](const auto& Type)
		{
			this->HandleEvent(Type);
			Handled = this->GUI.handleEvent(Type);
		});

		return Handled;
	}

	void GUISubsystem::Render()
	{
		GUI.draw();
	}

	void GUISubsystem::AddWidget(const tgui::Widget::Ptr& Widget)
	{
		GUI.add(Widget);
	}

	void GUISubsystem::RemoveWidget(const tgui::Widget::Ptr& Widget)
	{
		GUI.remove(Widget);
	}

	void GUISubsystem::HandleEvent(const sf::Event::Resized& Resize)
	{
		const sf::Vector2f DefualtSize = Window.getDefaultView().getSize();
		const sf::Vector2f Scale = sf::Vector2f(Resize.size).componentWiseDiv(DefualtSize);
		GUI.setRelativeView({ 0, 0, 1 / Scale.x, 1 / Scale.y });
	}

	void GUISubsystem::HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad)
	{
		if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) == GamepadButton::South)
		{
			GUI.handleEvent(sf::Event::MouseButtonPressed{ {}, sf::Mouse::getPosition(Window) });
		}
	}

	void GUISubsystem::HandleEvent(const sf::Event::JoystickButtonReleased& Gamepad)
	{
		if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) == GamepadButton::South)
		{
			GUI.handleEvent(sf::Event::MouseButtonReleased{ {}, sf::Mouse::getPosition(Window) });
		}
	}
}
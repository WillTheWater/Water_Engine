// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GUISubsystem.h"
#include "Framework/GameWindow.h"
#include "UI/Widget/Widget.h"
#include "Input/InputBinding.h"
#include "Utility/Log.h"

namespace we
{
	GUISubsystem::GUISubsystem(GameWindow& Window)
		: Window{ Window }
	{
	}

	void GUISubsystem::DestroyWidget(Widget* InWidget)
	{
		if (!InWidget) return;

		auto it = std::remove_if(Widgets.begin(), Widgets.end(),
			[InWidget](const shared<Widget>& W) { return W.get() == InWidget; });

		if (it != Widgets.end())
		{
			Widgets.erase(it);
		}
	}

	GUISubsystem::~GUISubsystem()
	{
		Clear();
	}

	void GUISubsystem::ProcessEvent(const sf::Event& Event)
	{
		Event.visit([this](const auto& Type)
			{
				this->HandleEvent(Type);
			});
	}

	void GUISubsystem::Update(float DeltaTime)
	{
		for (auto& Widget : Widgets)
		{
			if (Widget->IsVisible() && !Widget->GetParent())
			{
				Widget->Update(DeltaTime);
			}
		}
	}

	void GUISubsystem::Render()
	{
		for (auto& Widget : Widgets)
		{
			if (Widget->IsVisible() && !Widget->GetParent())
			{
				Widget->Render(Window);
			}
		}
	}

	void GUISubsystem::Clear()
	{
		Widgets.clear();
	}

	void GUISubsystem::HandleEvent(const sf::Event::MouseButtonPressed&)
	{
		bMousePressed = true;
	}

	void GUISubsystem::HandleEvent(const sf::Event::MouseButtonReleased&)
	{
		bMousePressed = false;
	}

	void GUISubsystem::HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad)
	{
		if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) == GamepadButton::South)
		{
			bMousePressed = true;
		}
	}

	void GUISubsystem::HandleEvent(const sf::Event::JoystickButtonReleased& Gamepad)
	{
		if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) == GamepadButton::South)
		{
			bMousePressed = false;
		}
	}
}
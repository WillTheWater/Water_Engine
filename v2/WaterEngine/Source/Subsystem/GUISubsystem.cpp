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
			if (Widget->IsVisible())
			{
				Widget->Update(DeltaTime);
			}
		}
	}

	void GUISubsystem::Render()
	{
		for (auto& Widget : Widgets)
		{
			if (Widget->IsVisible())
			{
				Widget->Render(Window);
			}
		}
	}

	void GUISubsystem::AddWidget(shared<Widget> InWidget)
	{
		if (!InWidget) return;
		auto it = std::find_if(Widgets.begin(), Widgets.end(),
			[&](const shared<Widget>& W)
			{
				return W->GetZOrder() > InWidget->GetZOrder();
			});

		Widgets.insert(it, std::move(InWidget));
	}

	void GUISubsystem::RemoveWidget(Widget* InWidget)
	{
		auto it = std::remove_if(Widgets.begin(), Widgets.end(),
			[InWidget](const shared<Widget>& W)
			{
				return W.get() == InWidget;
			});

		Widgets.erase(it, Widgets.end());
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
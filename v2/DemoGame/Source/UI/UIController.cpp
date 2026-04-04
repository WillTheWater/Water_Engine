// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/UIController.h"
#include "Input/InputBinding.h"
#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/GuiSubsystem.h"
#include "Utility/Log.h"
#include "Utility/Math.h"

namespace we
{
	UIController::UIController()
		: Input(InputSubsystem::Get())
	{
	}

	UIController::~UIController()
	{
		Disable();
	}

	void UIController::Enable()
	{
		if (bEnabled)
			return;

		bEnabled = true;
		bSouthWasPressed = false;
		
		GetCursor().SetVisibility(true);
		
		vec2u WindowSize = MakeGUI().GetWindowSize();
		WindowCursorPos = vec2f(WindowSize.x / 2.0f, WindowSize.y / 2.0f);
		
		event::MouseMoved e{ vec2i(static_cast<int>(WindowCursorPos.x), static_cast<int>(WindowCursorPos.y)) };
		MakeGUI().HandleEvent(event(e));
		
		vec2f WorldPos = WindowToWorld(WindowCursorPos);
		GetCursor().SetPosition(WorldPos);

		LOG("UIController enabled");
	}

	void UIController::Disable()
	{
		if (!bEnabled)
			return;

		bEnabled = false;
		
		LOG("UIController disabled");
	}

	void UIController::Tick(float DeltaTime)
	{
		if (!bEnabled)
			return;

		if (sf::Joystick::isConnected(0))
		{
			PollStick(DeltaTime);
			PollSouthButton();
		}
	}

	vec2f UIController::WindowToWorld(vec2f WindowPos)
	{
		vec2u WindowSize = MakeGUI().GetWindowSize();
		view CameraView = MakeGUI().GetCameraView();
		rectf Viewport = CameraView.getViewport();
		
		float normX = WindowPos.x / WindowSize.x;
		float normY = WindowPos.y / WindowSize.y;
		
		normX = (normX - Viewport.position.x) / Viewport.size.x;
		normY = (normY - Viewport.position.y) / Viewport.size.y;
		
		vec2f ViewSize = CameraView.getSize();
		vec2f ViewCenter = CameraView.getCenter();
		
		vec2f WorldPos;
		WorldPos.x = ViewCenter.x + (normX - 0.5f) * ViewSize.x;
		WorldPos.y = ViewCenter.y + (normY - 0.5f) * ViewSize.y;
		
		return WorldPos;
	}

	void UIController::PollStick(float DeltaTime)
	{
		float X = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

		if (std::abs(X) < Deadzone) X = 0.0f;
		if (std::abs(Y) < Deadzone) Y = 0.0f;

		if (X != 0.0f || Y != 0.0f)
		{
			WindowCursorPos.x += (X / 100.0f) * Speed * DeltaTime;
			WindowCursorPos.y += (Y / 100.0f) * Speed * DeltaTime;
			
			vec2u WindowSize = MakeGUI().GetWindowSize();
			WindowCursorPos.x = we::Clamp(WindowCursorPos.x, 0.0f, static_cast<float>(WindowSize.x));
			WindowCursorPos.y = we::Clamp(WindowCursorPos.y, 0.0f, static_cast<float>(WindowSize.y));
			
			vec2f WorldPos = WindowToWorld(WindowCursorPos);
			GetCursor().SetPosition(WorldPos);
			
			event::MouseMoved e{ vec2i(static_cast<int>(WindowCursorPos.x), static_cast<int>(WindowCursorPos.y)) };
			MakeGUI().HandleEvent(event(e));
		}
	}

	void UIController::PollSouthButton()
	{
		// Get hardware button ID using engine's mapping
		auto HardwareButton = Input::LogicToHardware(GamepadButton::South, 0);
		if (!HardwareButton)
			return;
		
		bool bPressed = sf::Joystick::isButtonPressed(0, *HardwareButton);
		
		if (bPressed && !bSouthWasPressed)
		{
			vec2i MousePos(static_cast<int>(WindowCursorPos.x), static_cast<int>(WindowCursorPos.y));
			
			event::MouseButtonPressed press{ sf::Mouse::Button::Left, MousePos };
			MakeGUI().HandleEvent(event(press));
			
			event::MouseButtonReleased release{ sf::Mouse::Button::Left, MousePos };
			MakeGUI().HandleEvent(event(release));
			
			LOG("UI Click at window {:.0f}, {:.0f}", WindowCursorPos.x, WindowCursorPos.y);
		}
		
		bSouthWasPressed = bPressed;
	}
}

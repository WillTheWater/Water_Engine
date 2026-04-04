// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Input/ControllerTest.h"
#include "Input/InputBinding.h"
#include "Utility/Log.h"

namespace we
{
	ControllerTest::ControllerTest()
		: Input(InputSubsystem::Get())
	{
	}

	ControllerTest::~ControllerTest()
	{
		EndTest();
	}

	void ControllerTest::BeginTest(int InGamepadId)
	{
		GamepadId = InGamepadId;
		bConnected = true;
		BindButtons();
		LOG("Controller test started on gamepad {}", GamepadId);
	}

	void ControllerTest::EndTest()
	{
		bConnected = false;
		UnbindButtons();
		GamepadId = -1;
	}

	void ControllerTest::Tick()
	{
		if (!bConnected || GamepadId < 0)
			return;

		PollAxes();
	}

	void ControllerTest::BindButtons()
	{
		if (GamepadId < 0)
			return;

		// Map hardware buttons to actions (Input::Bind), then actions to callbacks (BindAction)
		Input.Bind(static_cast<int>(GamepadButton::South), Input::Gamepad{ GamepadButton::South, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::East), Input::Gamepad{ GamepadButton::East, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::North), Input::Gamepad{ GamepadButton::North, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::West), Input::Gamepad{ GamepadButton::West, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::L1), Input::Gamepad{ GamepadButton::L1, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::R1), Input::Gamepad{ GamepadButton::R1, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::Select), Input::Gamepad{ GamepadButton::Select, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::Start), Input::Gamepad{ GamepadButton::Start, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::L3), Input::Gamepad{ GamepadButton::L3, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::R3), Input::Gamepad{ GamepadButton::R3, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::DPadUp), Input::Gamepad{ GamepadButton::DPadUp, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::DPadDown), Input::Gamepad{ GamepadButton::DPadDown, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::DPadLeft), Input::Gamepad{ GamepadButton::DPadLeft, GamepadId });
		Input.Bind(static_cast<int>(GamepadButton::DPadRight), Input::Gamepad{ GamepadButton::DPadRight, GamepadId });

		SouthBinding = Input.BindAction(static_cast<int>(GamepadButton::South), this, &ControllerTest::OnSouth);
		EastBinding = Input.BindAction(static_cast<int>(GamepadButton::East), this, &ControllerTest::OnEast);
		NorthBinding = Input.BindAction(static_cast<int>(GamepadButton::North), this, &ControllerTest::OnNorth);
		WestBinding = Input.BindAction(static_cast<int>(GamepadButton::West), this, &ControllerTest::OnWest);
		L1Binding = Input.BindAction(static_cast<int>(GamepadButton::L1), this, &ControllerTest::OnL1);
		R1Binding = Input.BindAction(static_cast<int>(GamepadButton::R1), this, &ControllerTest::OnR1);
		SelectBinding = Input.BindAction(static_cast<int>(GamepadButton::Select), this, &ControllerTest::OnSelect);
		StartBinding = Input.BindAction(static_cast<int>(GamepadButton::Start), this, &ControllerTest::OnStart);
		L3Binding = Input.BindAction(static_cast<int>(GamepadButton::L3), this, &ControllerTest::OnL3);
		R3Binding = Input.BindAction(static_cast<int>(GamepadButton::R3), this, &ControllerTest::OnR3);
		DPadUpBinding = Input.BindAction(static_cast<int>(GamepadButton::DPadUp), this, &ControllerTest::OnDPadUp);
		DPadDownBinding = Input.BindAction(static_cast<int>(GamepadButton::DPadDown), this, &ControllerTest::OnDPadDown);
		DPadLeftBinding = Input.BindAction(static_cast<int>(GamepadButton::DPadLeft), this, &ControllerTest::OnDPadLeft);
		DPadRightBinding = Input.BindAction(static_cast<int>(GamepadButton::DPadRight), this, &ControllerTest::OnDPadRight);
	}

	void ControllerTest::UnbindButtons()
	{
		SouthBinding = BindingHandle();
		EastBinding = BindingHandle();
		NorthBinding = BindingHandle();
		WestBinding = BindingHandle();
		L1Binding = BindingHandle();
		R1Binding = BindingHandle();
		SelectBinding = BindingHandle();
		StartBinding = BindingHandle();
		L3Binding = BindingHandle();
		R3Binding = BindingHandle();
		DPadUpBinding = BindingHandle();
		DPadDownBinding = BindingHandle();
		DPadLeftBinding = BindingHandle();
		DPadRightBinding = BindingHandle();
	}

	void ControllerTest::PollAxes()
	{
		if (!sf::Joystick::isConnected(GamepadId))
			return;

		float NewX = sf::Joystick::getAxisPosition(GamepadId, sf::Joystick::Axis::X);
		float NewY = sf::Joystick::getAxisPosition(GamepadId, sf::Joystick::Axis::Y);
		float NewZ = sf::Joystick::getAxisPosition(GamepadId, sf::Joystick::Axis::Z);
		float NewR = sf::Joystick::getAxisPosition(GamepadId, sf::Joystick::Axis::R);
		float NewU = sf::Joystick::getAxisPosition(GamepadId, sf::Joystick::Axis::U);
		float NewV = sf::Joystick::getAxisPosition(GamepadId, sf::Joystick::Axis::V);
		float NewPovX = sf::Joystick::getAxisPosition(GamepadId, sf::Joystick::Axis::PovX);
		float NewPovY = sf::Joystick::getAxisPosition(GamepadId, sf::Joystick::Axis::PovY);

		// LOG axis changes only when they cross a threshold (deadzone)
		const float Threshold = 15.0f;
		const float TriggerThreshold = 1.0f; // Triggers rest at 0, go to +/-100
		
		if (std::abs(NewX) > Threshold && std::abs(AxisX) <= Threshold)
			LOG("Left Stick X: {:.0f}", NewX);
		if (std::abs(NewY) > Threshold && std::abs(AxisY) <= Threshold)
			LOG("Left Stick Y: {:.0f}", NewY);
		if (std::abs(NewU) > Threshold && std::abs(AxisU) <= Threshold)
			LOG("Right Stick X: {:.0f}", NewU);
		if (std::abs(NewV) > Threshold && std::abs(AxisV) <= Threshold)
			LOG("Right Stick Y: {:.0f}", NewV);

		// L2/R2 Triggers on Xbox: Z axis is combined (L2 positive, R2 negative)
		if (NewZ > TriggerThreshold && AxisZ <= TriggerThreshold)
			LOG("Trigger: L2 (Z: {:.0f})", NewZ);
		if (NewZ < -TriggerThreshold && AxisZ >= -TriggerThreshold)
			LOG("Trigger: R2 (Z: {:.0f})", NewZ);

		// D-Pad as POV axes (some Xbox controllers use these instead of buttons)
		if (NewPovY > Threshold && PovY <= Threshold)
			LOG("D-Pad Down (PovY: {:.0f})", NewPovY);
		if (NewPovY < -Threshold && PovY >= -Threshold)
			LOG("D-Pad Up (PovY: {:.0f})", NewPovY);
		if (NewPovX > Threshold && PovX <= Threshold)
			LOG("D-Pad Right (PovX: {:.0f})", NewPovX);
		if (NewPovX < -Threshold && PovX >= -Threshold)
			LOG("D-Pad Left (PovX: {:.0f})", NewPovX);

		AxisX = NewX;
		AxisY = NewY;
		AxisZ = NewZ;
		AxisR = NewR;
		AxisU = NewU;
		AxisV = NewV;
		PovX = NewPovX;
		PovY = NewPovY;
	}

	// Button callbacks - just LOG when pressed
	void ControllerTest::OnSouth()    { LOG("Button: South (A/Cross)"); }
	void ControllerTest::OnEast()     { LOG("Button: East (B/Circle)"); }
	void ControllerTest::OnNorth()    { LOG("Button: North (Y/Triangle)"); }
	void ControllerTest::OnWest()     { LOG("Button: West (X/Square)"); }
	void ControllerTest::OnL1()       { LOG("Button: L1"); }
	void ControllerTest::OnR1()       { LOG("Button: R1"); }
	void ControllerTest::OnSelect()   { LOG("Button: Select/View"); }
	void ControllerTest::OnStart()    { LOG("Button: Start/Menu"); }
	void ControllerTest::OnL3()       { LOG("Button: L3 (Left Stick Click)"); }
	void ControllerTest::OnR3()       { LOG("Button: R3 (Right Stick Click)"); }
	void ControllerTest::OnDPadUp()   { LOG("Button: D-Pad Up"); }
	void ControllerTest::OnDPadDown() { LOG("Button: D-Pad Down"); }
	void ControllerTest::OnDPadLeft() { LOG("Button: D-Pad Left"); }
	void ControllerTest::OnDPadRight(){ LOG("Button: D-Pad Right"); }
}
// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Subsystem/InputSubsystem.h"

namespace we
{
	class ControllerTest
	{
	public:
		ControllerTest();
		~ControllerTest();

		void BeginTest(int InGamepadId);
		void EndTest();

		void Tick();

	private:
		void BindButtons();
		void UnbindButtons();
		void PollAxes();

		// Button callbacks
		void OnSouth();
		void OnEast();
		void OnNorth();
		void OnWest();
		void OnL1();
		void OnR1();
		void OnSelect();
		void OnStart();
		void OnL3();
		void OnR3();
		void OnDPadUp();
		void OnDPadDown();
		void OnDPadLeft();
		void OnDPadRight();

		InputSubsystem& Input;
		int GamepadId = -1;
		bool bConnected = false;

		// Axis values
		float AxisX = 0.0f;
		float AxisY = 0.0f;
		float AxisZ = 0.0f;  // L2 trigger (Xbox)
		float AxisR = 0.0f;  // R2 trigger (Xbox)
		float AxisU = 0.0f;
		float AxisV = 0.0f;
		float PovX = 0.0f;   // D-Pad X (some controllers)
		float PovY = 0.0f;   // D-Pad Y (some controllers)

		// Bindings
		BindingHandle SouthBinding;
		BindingHandle EastBinding;
		BindingHandle NorthBinding;
		BindingHandle WestBinding;
		BindingHandle L1Binding;
		BindingHandle R1Binding;
		BindingHandle SelectBinding;
		BindingHandle StartBinding;
		BindingHandle L3Binding;
		BindingHandle R3Binding;
		BindingHandle DPadUpBinding;
		BindingHandle DPadDownBinding;
		BindingHandle DPadLeftBinding;
		BindingHandle DPadRightBinding;
	};
}
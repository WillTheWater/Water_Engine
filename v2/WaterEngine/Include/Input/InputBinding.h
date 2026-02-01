// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include <SFML/Window.hpp>

#include "Utility/CoreMinimal.h"
#include "EngineConfig.h"

namespace we
{
	enum class GamepadButton
	{
		West, South, East, North,
		L1, R1, L2, R2,
		Select, Start, L3, R3,
		DPadUp, DPadDown, DPadLeft, DPadRight
	};

	namespace Input
	{
		struct Keyboard
		{
			sf::Keyboard::Scan Key;
			bool operator==(const Keyboard& OtherKey) const { return Key == OtherKey.Key; }
		};

		struct Mouse
		{
			sf::Mouse::Button Button;
			bool operator==(const Mouse& OtherKey) const { return Button == OtherKey.Button; }
		};

		struct Gamepad
		{
			GamepadButton Button;
			unsigned int GamepadID = 0;
			bool operator==(const Gamepad& OtherKey) const
			{
				return Button == OtherKey.Button && GamepadID == OtherKey.GamepadID;
			}
		};

		struct JoystickAxis
		{
			sf::Joystick::Axis Axis;
			float AxisThreshold = EC.JoystickDeadzone;
			int JoystickID = 0;
			bool operator==(const JoystickAxis& OtherKey) const
			{
				return Axis == OtherKey.Axis && JoystickID == OtherKey.JoystickID;
			}
		};

		using Binding = std::variant<Keyboard, Mouse, Gamepad, JoystickAxis>;

		optional<int> LogicToHardware(GamepadButton Button, int HardwareID);
		optional<GamepadButton> HardwareToLogic(int Button, int HardwareID);

		inline bool BindingsEqual(const Binding& A, const Binding& B)
		{
			return std::visit([](const auto& x, const auto& y) -> bool {
				if constexpr (std::is_same_v<decltype(x), decltype(y)>) {
					return x == y;
				}
				return false;
				}, A, B);
		}
	}
}
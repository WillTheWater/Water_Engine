// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include <SFML/Window.hpp>
#include "Core/CoreMinimal.h"

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
        struct Keyboard { sf::Keyboard::Scan Key; };
        struct Mouse { sf::Mouse::Button Button; };
        struct Gamepad { GamepadButton Button; int id = 0; };
        struct Axis { sf::Joystick::Axis Axis; float Threshold = 0.15f; int id = 0; };

        using Binding = std::variant<Keyboard, Mouse, Gamepad, Axis>;

        optional<int> LogicToHardware(GamepadButton Button, int HardwareID);
        optional<GamepadButton> HardwareToLogic(int Button, int HardwareID);
    }
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Input/InputBinding.h"

namespace we
{
    namespace Mapping
    {
        const arr<int, 16> PlayStation = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,16,17,18,19 };
        const arr<int, 16> Xbox = { 2, 0, 1, 3, 4, 5,-1,-1, 6, 7, 8, 9,12, 13, 14, 15 };

        const int SonyVID = 0x054C;
        const int MicrosoftVID = 0x045E;

        const auto& GetGamepadMapping(int GamepadID)
        {
            const int VendorId = sf::Joystick::getIdentification(GamepadID).vendorId;
            return (VendorId == MicrosoftVID) ? Xbox : PlayStation;
        }
    }

    optional<int> we::Input::LogicToHardware(GamepadButton Button, int HardwareID)
    {
        const auto& Mapping = Mapping::GetGamepadMapping(HardwareID);
        const int Value = Mapping[(int)Button];
        return (Value != -1) ? std::optional(Value) : std::nullopt;
    }

    optional<GamepadButton> Input::HardwareToLogic(int Button, int HardwareID)
    {
        const auto& Mapping = Mapping::GetGamepadMapping(HardwareID);
        for (std::size_t i = 0; i < Mapping.size(); i++)
        {
            if (Mapping[i] == Button)
            {
                return GamepadButton(i);
            }
        }
        return std::nullopt;
    }
}
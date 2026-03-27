// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <SFML/Window/Event.hpp>

namespace we
{
    class WindowSubsystem;

    struct WindowEventHandler
    {
        WindowSubsystem& Window;

        // Window events
        void operator()(const event::Closed&);
        void operator()(const event::Resized&);
        void operator()(const event::FocusLost&);
        void operator()(const event::FocusGained&);

        // Gamepad events
        void operator()(const event::JoystickConnected&);
        void operator()(const event::JoystickDisconnected&);
        void operator()(const event::JoystickButtonPressed&);
        void operator()(const event::JoystickButtonReleased&);

        // Keyboard events
        void operator()(const event::KeyPressed&);
        void operator()(const event::KeyReleased&);

        // Mouse events
        void operator()(const event::MouseMoved&);
        void operator()(const event::MouseButtonPressed&);
        void operator()(const event::MouseButtonReleased&);

        void operator()(const auto&) {}
    };
}
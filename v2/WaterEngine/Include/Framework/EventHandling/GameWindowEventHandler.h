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

    // =========================================================================
    // GameWindowEventHandler - Dispatches SFML window events to appropriate handlers
    // Uses std::visit pattern for type-safe event handling
    // =========================================================================
    struct GameWindowEventHandler
    {
        WindowSubsystem& Window;

        // Window events
        void operator()(const sf::Event::Closed&);
        void operator()(const sf::Event::Resized&);
        void operator()(const sf::Event::FocusLost&);
        void operator()(const sf::Event::FocusGained&);

        // Gamepad events
        void operator()(const sf::Event::JoystickConnected&);
        void operator()(const sf::Event::JoystickDisconnected&);
        void operator()(const sf::Event::JoystickButtonPressed&);
        void operator()(const sf::Event::JoystickButtonReleased&);

        // Keyboard events
        void operator()(const sf::Event::KeyPressed&);
        void operator()(const sf::Event::KeyReleased&);

        // Mouse events
        void operator()(const sf::Event::MouseMoved&);
        void operator()(const sf::Event::MouseButtonPressed&);
        void operator()(const sf::Event::MouseButtonReleased&);

        // Catch-all for unhandled event types
        void operator()(const auto&) {}
    };

} // namespace we

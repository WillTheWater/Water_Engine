// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <SFML/Window/Event.hpp>

namespace we
{
    class Editor;

    struct EditorEventHandler
    {
        Editor& EditorInstance;

        // Window events (editor-specific handling)
        void operator()(const sf::Event::Closed&);
        void operator()(const sf::Event::Resized&);
        void operator()(const sf::Event::FocusLost&);
        void operator()(const sf::Event::FocusGained&);

        // Keyboard events
        void operator()(const sf::Event::KeyPressed&);
        void operator()(const sf::Event::KeyReleased&);

        // Mouse events
        void operator()(const sf::Event::MouseMoved&);
        void operator()(const sf::Event::MouseButtonPressed&);
        void operator()(const sf::Event::MouseButtonReleased&);
        void operator()(const sf::Event::MouseWheelScrolled&);

        // Gamepad events (editor typically doesn't use these, but handle for completeness)
        void operator()(const sf::Event::JoystickConnected&);
        void operator()(const sf::Event::JoystickDisconnected&);
        void operator()(const sf::Event::JoystickButtonPressed&);
        void operator()(const sf::Event::JoystickButtonReleased&);
        void operator()(const sf::Event::JoystickMoved&);

        // Text input
        void operator()(const sf::Event::TextEntered&);

        // Catch-all for unhandled event types
        void operator()(const auto&) {}
    };
}

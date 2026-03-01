// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Input/WindowEventHandler.h"
#include "Subsystem/WindowSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    // =========================================================================
    // Window Events
    // =========================================================================
    void WindowEventHandler::operator()(const sf::Event::Closed&)
    {
        Window.close();
    }

    void WindowEventHandler::operator()(const sf::Event::Resized&)
    {
        Window.onResize();
    }

    void WindowEventHandler::operator()(const sf::Event::FocusLost&)
    {
        // Window lost focus - could pause audio or reduce update rate here
    }

    void WindowEventHandler::operator()(const sf::Event::FocusGained&)
    {
        // Window gained focus - resume normal operation
    }

    // =========================================================================
    // Gamepad Events
    // =========================================================================
    void WindowEventHandler::operator()(const sf::Event::JoystickConnected&)
    {
        // Handled by InputSubsystem
    }

    void WindowEventHandler::operator()(const sf::Event::JoystickDisconnected&)
    {
        // Handled by InputSubsystem
    }

    void WindowEventHandler::operator()(const sf::Event::JoystickButtonPressed&)
    {
        // Handled by InputSubsystem
    }

    void WindowEventHandler::operator()(const sf::Event::JoystickButtonReleased&)
    {
        // Handled by InputSubsystem
    }

    // =========================================================================
    // Keyboard Events
    // =========================================================================
    void WindowEventHandler::operator()(const sf::Event::KeyPressed& Key)
    {
        // F11: Toggle fullscreen
        if (Key.scancode == sf::Keyboard::Scan::F11)
        {
            Window.EventToggleBorderlessFullscreen();
        }
    }

    void WindowEventHandler::operator()(const sf::Event::KeyReleased&)
    {
        // Handled by InputSubsystem
    }

    // =========================================================================
    // Mouse Events
    // =========================================================================
    void WindowEventHandler::operator()(const sf::Event::MouseMoved&)
    {
        // Handled by InputSubsystem or Editor
    }

    void WindowEventHandler::operator()(const sf::Event::MouseButtonPressed&)
    {
        // Handled by InputSubsystem
    }

    void WindowEventHandler::operator()(const sf::Event::MouseButtonReleased&)
    {
        // Handled by InputSubsystem
    }
}

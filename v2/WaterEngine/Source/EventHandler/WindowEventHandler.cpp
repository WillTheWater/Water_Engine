// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EventHandler/WindowEventHandler.h"
#include "Subsystem/WindowSubsystem.h"

namespace we
{
    // =========================================================================
    // Window Events
    // =========================================================================
    void WindowEventHandler::operator()(const event::Closed&)
    {
        Window.close();
    }

    void WindowEventHandler::operator()(const event::Resized&)
    {
        Window.EventResize();
    }

    void WindowEventHandler::operator()(const event::FocusLost&)
    {
       
    }

    void WindowEventHandler::operator()(const event::FocusGained&)
    {
      
    }

    // =========================================================================
    // Gamepad Events
    // =========================================================================
    void WindowEventHandler::operator()(const event::JoystickConnected&)
    {
       
    }

    void WindowEventHandler::operator()(const event::JoystickDisconnected&)
    {
       
    }

    void WindowEventHandler::operator()(const event::JoystickButtonPressed&)
    {
       
    }

    void WindowEventHandler::operator()(const event::JoystickButtonReleased&)
    {
        
    }

    // =========================================================================
    // Keyboard Events
    // =========================================================================
    void WindowEventHandler::operator()(const event::KeyPressed& Key)
    {
        // F11: Toggle fullscreen
        if (Key.scancode == sf::Keyboard::Scan::F11)
        {
            //Window.EventToggleBorderlessFullscreen();
        }
    }

    void WindowEventHandler::operator()(const event::KeyReleased&)
    {
        
    }

    // =========================================================================
    // Mouse Events
    // =========================================================================
    void WindowEventHandler::operator()(const event::MouseMoved&)
    {
        
    }

    void WindowEventHandler::operator()(const event::MouseButtonPressed&)
    {
       
    }

    void WindowEventHandler::operator()(const event::MouseButtonReleased&)
    {
       
    }
}
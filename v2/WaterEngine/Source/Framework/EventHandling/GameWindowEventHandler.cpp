// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/EventHandling/GameWindowEventHandler.h"
#include "Subsystem/WindowSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    // =========================================================================
    // Window Events
    // =========================================================================
    void GameWindowEventHandler::operator()(const sf::Event::Closed&)
    {
        Window.EventWindowClose();
    }

    void GameWindowEventHandler::operator()(const sf::Event::Resized&)
    {
        // Handled by WindowSubsystem::onResize() override
    }

    void GameWindowEventHandler::operator()(const sf::Event::FocusLost&)
    {
        // TODO: Pause game when focus lost (optional)
    }

    void GameWindowEventHandler::operator()(const sf::Event::FocusGained&)
    {
        // TODO: Resume game when focus gained (optional)
    }

    // =========================================================================
    // Input Events - Gamepad
    // =========================================================================
    void GameWindowEventHandler::operator()(const sf::Event::JoystickConnected&)
    {
        LOG("Gamepad Connected");
    }

    void GameWindowEventHandler::operator()(const sf::Event::JoystickDisconnected&)
    {
        LOG("Gamepad Disconnected");
    }

    void GameWindowEventHandler::operator()(const sf::Event::JoystickButtonPressed&)
    {
        // Handled by InputSubsystem
    }

    void GameWindowEventHandler::operator()(const sf::Event::JoystickButtonReleased&)
    {
        // Handled by InputSubsystem
    }

    // =========================================================================
    // Input Events - Keyboard
    // =========================================================================
    void GameWindowEventHandler::operator()(const sf::Event::KeyPressed& Key)
    {
        // F11: Toggle fullscreen
        if (Key.scancode == sf::Keyboard::Scan::F11)
        {
            Window.EventToggleBorderlessFullscreen();
        }
        
        // Other keys handled by InputSubsystem
    }

    void GameWindowEventHandler::operator()(const sf::Event::KeyReleased&)
    {
        // Handled by InputSubsystem
    }

    // =========================================================================
    // Input Events - Mouse
    // =========================================================================
    void GameWindowEventHandler::operator()(const sf::Event::MouseMoved&)
    {
        // Handled by WaterEngine::ProcessEvents() -> CursorSubsystem
    }

    void GameWindowEventHandler::operator()(const sf::Event::MouseButtonPressed&)
    {
        // Handled by InputSubsystem
    }

    void GameWindowEventHandler::operator()(const sf::Event::MouseButtonReleased&)
    {
        // Handled by InputSubsystem
    }

} // namespace we

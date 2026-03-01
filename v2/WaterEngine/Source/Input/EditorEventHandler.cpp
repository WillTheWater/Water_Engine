// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Input/EditorEventHandler.h"
#include "Framework/Editor.h"
#include "Subsystem/RenderSubsystem.h"
#include "Utility/Log.h"

// ImGui-SFML for event processing
#include <imgui.h>
#include <imgui-SFML.h>

namespace we
{
    // =========================================================================
    // Window Events
    // =========================================================================
    void EditorEventHandler::operator()(const sf::Event::Closed&)
    {
        // Window close is handled by WindowSubsystem
    }

    void EditorEventHandler::operator()(const sf::Event::Resized&)
    {
        // Window resize is handled by WindowSubsystem
    }

    void EditorEventHandler::operator()(const sf::Event::FocusLost&)
    {
        // Editor lost focus - could pause certain operations
    }

    void EditorEventHandler::operator()(const sf::Event::FocusGained&)
    {
        // Editor gained focus
    }

    // =========================================================================
    // Keyboard Events
    // =========================================================================
    void EditorEventHandler::operator()(const sf::Event::KeyPressed& e)
    {
        // ESC to exit Play mode and return to Editor
        if (e.code == sf::Keyboard::Key::Escape)
        {
            // Handled directly in WaterEngine to switch modes
            // Mark as consumed by not passing to ImGui if needed
        }
    }

    void EditorEventHandler::operator()(const sf::Event::KeyReleased&)
    {
        // Key releases typically don't need special handling for editor
    }

    // =========================================================================
    // Mouse Events
    // =========================================================================
    void EditorEventHandler::operator()(const sf::Event::MouseMoved&)
    {
        // Mouse movement handled by ImGui-SFML and viewport input
    }

    void EditorEventHandler::operator()(const sf::Event::MouseButtonPressed&)
    {
        // Mouse button presses handled by ImGui-SFML
    }

    void EditorEventHandler::operator()(const sf::Event::MouseButtonReleased&)
    {
        // Mouse button releases handled by ImGui-SFML
    }

    void EditorEventHandler::operator()(const sf::Event::MouseWheelScrolled&)
    {
        // Mouse wheel handled by ImGui-SFML and viewport zoom
    }

    // =========================================================================
    // Gamepad Events (Editor doesn't typically use gamepad)
    // =========================================================================
    void EditorEventHandler::operator()(const sf::Event::JoystickConnected&)
    {
        // Handled by WindowSubsystem for logging
    }

    void EditorEventHandler::operator()(const sf::Event::JoystickDisconnected&)
    {
        // Handled by WindowSubsystem for logging
    }

    void EditorEventHandler::operator()(const sf::Event::JoystickButtonPressed&)
    {
        // Editor doesn't use gamepad buttons
    }

    void EditorEventHandler::operator()(const sf::Event::JoystickButtonReleased&)
    {
        // Editor doesn't use gamepad buttons
    }

    void EditorEventHandler::operator()(const sf::Event::JoystickMoved&)
    {
        // Editor doesn't use gamepad axes
    }

    // =========================================================================
    // Text Input
    // =========================================================================
    void EditorEventHandler::operator()(const sf::Event::TextEntered&)
    {
        // Text input handled by ImGui-SFML
    }
}

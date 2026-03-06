// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <SFML/Graphics.hpp>
#include "EventHandler/WindowEventHandler.h"
#include "Utility/Delegate.h"

namespace we
{
    class WindowSubsystem : public sf::RenderWindow
    {
    public:
        explicit WindowSubsystem();

        void HandleEvent(const event& Event);

    public:
        Delegate<vec2u> OnResize;
        Delegate<vec2f> OnMouseMove;

    private:
        friend WindowEventHandler;

    private:
        void EventResize();
        void EventMouseMoved(vec2f NewPosition);
    };
}
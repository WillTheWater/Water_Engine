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
        void SetIcon(const string& Path);

        void ToggleFullscreen();
        void SetFullscreen(bool bEnabled);
        bool IsFullscreen() const { return bFullscreen; }

    public:
        Delegate<vec2u> OnResize;
        Delegate<vec2f> OnMouseMove;

    private:
        friend WindowEventHandler;

        void EventResize();
        void EventMouseMoved(vec2f NewPosition);

        bool bFullscreen = false;
        vec2u WindowedSize{1920, 1080};
    };
}

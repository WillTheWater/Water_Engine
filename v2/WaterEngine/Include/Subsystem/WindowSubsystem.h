// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <SFML/Graphics.hpp>
#include "Framework/EventHandling/GameWindowEventHandler.h"

namespace we
{
    struct WindowConfig
    {
        stringView WindowName;
        vec2f      RenderResolution;
        vec2f      AspectRatio;
        vec2f      WindowMinimumSize;
        stringView WindowIcon;
        bool       FullscreenMode;
        bool       VsyncEnabled;
        float      TargetFPS;
        bool       EnableKeyRepeat;
        bool       DisableSFMLLogs;
    };

    class WindowSubsystem : public sf::RenderWindow
    {
    public:
        explicit WindowSubsystem(const WindowConfig& Config);

        // Event Handling
        void HandleEvent(const sf::Event& Event);

        // Window State
        void SetFullscreen(bool bFullscreen);
        bool IsFullscreen() const { return bIsFullscreen; }

        // VSync
        void SetVSync(bool bEnabled);
        bool IsVSync() const { return Config.VsyncEnabled; }

        // Input
        vec2f GetMousePosition() const;

    protected:
        // SFML callback when window is resized
        void onResize() override;

    private:
        // Aspect ratio helper
        vec2u CalculateAspectRatioSize(const vec2u& Size) const;

        // Window creation
        void CreateGameWindow(const sf::VideoMode& Mode, uint Style, sf::State State = sf::State::Windowed);
        void ApplyWindowSettings();
        void SetWindowIcon();

        // Event handlers (called by GameWindowEventHandler)
        void EventWindowClose();
        void EventToggleBorderlessFullscreen();

    private:
        bool         bIsFullscreen;
        bool         bIsResizing = false;  // Prevents recursive resize events
        WindowConfig Config;

    private:
        friend GameWindowEventHandler;
    };

}

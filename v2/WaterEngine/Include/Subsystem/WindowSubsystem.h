// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"
#include <SFML/Graphics.hpp>
#include "Input/WindowEventHandler.h"

namespace we
{
    class WindowSubsystem : public sf::RenderWindow
    {
    public:
        // Create window using config values
        explicit WindowSubsystem(const EngineConfig::WindowConfig& Config);
        
        void HandleEvent(const sf::Event& Event);
        // Handle resize events - enforces aspect ratio and minimum size
        void onResize() override;
        
        // Toggle fullscreen mode
        void SetFullscreen(bool bFullscreen);
        bool IsFullscreen() const { return bIsFullscreen; }
        
        // Toggle VSync
        void SetVSync(bool bEnabled);
        
        // Get mouse position in window coordinates
        vec2f GetMousePosition() const;

    private:
        // VSync takes precedence: if enabled, TargetFPS is ignored
        void ConfigureFrameLimit(const EngineConfig::WindowConfig& Config);
        
        // Calculate size that maintains aspect ratio
        vec2u CalculateAspectRatioSize(const vec2u& Size) const;
        
        // Window creation helpers
        void CreateGameWindow(const sf::VideoMode& Mode, uint Style, sf::State State);
        void ApplyWindowSettings();
        void SetWindowIcon();
        
        // Event handlers
        void EventToggleBorderlessFullscreen();
        void EventWindowClose() { close(); }

    private:
        friend WindowEventHandler;
        bool bIsFullscreen = false;
        bool bIsResizing = false;  // Prevent recursive resize events
        EngineConfig::WindowConfig Config;
    };
}

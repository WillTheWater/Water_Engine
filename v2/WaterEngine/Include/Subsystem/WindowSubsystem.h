// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"
#include <SFML/Graphics.hpp>

namespace we
{
    class WindowSubsystem : public sf::RenderWindow
    {
    public:
        // Create window using config values
        explicit WindowSubsystem(const EngineConfig::WindowConfig& Config);

    private:
        // VSync takes precedence: if enabled, TargetFPS is ignored
        void ConfigureFrameLimit(const EngineConfig::WindowConfig& Config);
    };
}

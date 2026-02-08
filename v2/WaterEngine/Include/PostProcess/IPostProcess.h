// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace we
{
    class IPostProcess
    {
    public:
        virtual ~IPostProcess() = default;

        virtual void Apply(const sf::Texture& Input, sf::RenderTarget& Output) = 0;

    };
}
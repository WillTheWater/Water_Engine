// =============================================================================
// Water Engine v2.1.2
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace we
{
    class IPostProcess
    {
    public:
        virtual ~IPostProcess() = default;

        virtual void Update(float DeltaTime) {}
        virtual void Apply(const texture& Input, renderTarget& Output) = 0;
    };
}
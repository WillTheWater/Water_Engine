// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    enum class ERenderLayer : uint8
    {
        World,
        WorldUI,
        ScreenUI,
        Cursor
    };

    class RenderSubsystem
    {
    public:
        explicit RenderSubsystem();

        void BeginFrame();
        void Draw(const drawable& RenderObject, ERenderLayer Layer);
        void EndFrame();
        
        sprite GetCompositeSprite() const;
        vec2u GetRenderResolution() const { return RenderResolution; }

        void SetTargetSize(vec2u Size);

        // GUI render target access
        sf::RenderTarget& GetWorldUITarget() { return WorldUIRenderTarget; }
        sf::RenderTarget& GetScreenUITarget() { return ScreenUIRenderTarget; }

    private:
        renderTexture WorldRenderTarget;
        renderTexture WorldUIRenderTarget;
        renderTexture ScreenUIRenderTarget;
        renderTexture CursorRenderTarget;
        renderTexture Composite;

        vec2u RenderResolution;
        bool bNeedsComposite;

    private:
        void CreateRenderTargets();
        void ClearRenderTargets();
        void CompositeLayers();
    };
}
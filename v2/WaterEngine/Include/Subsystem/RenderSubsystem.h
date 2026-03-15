// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/PostProcess/IPostProcess.h"

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
        renderTarget& GetWorldUITarget() { return WorldUIRenderTarget; }
        renderTarget& GetScreenUITarget() { return ScreenUIRenderTarget; }

    private:
        renderTexture WorldRenderTarget;
        renderTexture WorldPostProcessTarget;
        vector<unique<IPostProcess>> WorldPostProcessEffects;

        renderTexture WorldUIRenderTarget;
        renderTexture ScreenUIRenderTarget;
        renderTexture CursorRenderTarget;

        renderTexture Composite;
        renderTexture CompositePostProcessTarget;
        vector<unique<IPostProcess>> CompositePostProcessEffects;

        vec2u RenderResolution;
        bool bNeedsComposite;

    private:
        void CreateRenderTargets();
        void ClearRenderTargets();
        void PostProcess(renderTexture* Input, renderTexture* Output, vector<unique<IPostProcess>>& Effects);
        void ApplyPostProcess(renderTexture& MainTarget, renderTexture& PostProcessTarget, vector<unique<IPostProcess>>& Effects);
        void CompositeLayers();
    };
}
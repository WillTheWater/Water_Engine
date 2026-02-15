// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "PostProcess/IPostProcess.h"

namespace we
{
    enum class ERenderLayer : uint8
    {
        Game,
        UI,
        Cursor
    };

    class RenderSubsystem
    {
    public:
        RenderSubsystem();

        void Draw(const drawable& RenderObject, ERenderLayer Layer = ERenderLayer::Game);

        void SetRenderView(const view& RenderView);
        renderTexture* GetTargetForLayer(ERenderLayer Layer);
        void SetLayerView(ERenderLayer Layer, const view& View);
        vec2f GetRenderSize() const { return vec2f(GameRenderTarget.getSize()); }
        void ResetRenderView();

        // Decoupled: Returns the final composited texture
        const texture& FinishRender();

        // Window-agnostic coordinate conversion using view
        vec2f WindowToRenderCoords(vec2i WindowPixel, const view& View) const;
        vec2i RenderToWindowCoords(vec2f RenderPos, const view& View) const;

        // Called by engine when window resizes
        void OnWindowResized(vec2u NewWindowSize);

    private:
        // Game Render
        renderTexture GameRenderTarget;
        renderTexture GamePostProcessTarget;
        vector<unique<IPostProcess>> GamePostProcessEffects;

        // UI Render
        renderTexture UIRenderTarget;
        renderTexture UIPostProcessTarget;
        vector<unique<IPostProcess>> UIPostEffects;

        // Cursor Render
        renderTexture CursorRenderTarget;
        renderTexture CursorPostProcessTarget;
        vector<unique<IPostProcess>> CursorPostProcessEffects;

        // Final composite target (the decoupling point)
        renderTexture CompositeTarget;

        vec2u CurrentWindowSize;

    private:
        friend class WaterEngine;

        void ApplyRenderView();
        void ProcessPostEffects(renderTexture* Input, renderTexture* Output, vector<unique<IPostProcess>>& Effects);
        void CompositeLayers();
        void Initialize();
        void StartRender();
    };
}
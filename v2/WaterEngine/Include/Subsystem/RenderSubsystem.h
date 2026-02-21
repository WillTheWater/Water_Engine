// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "PostProcess/IPostProcess.h"

#include "Subsystem/CameraSubsystem.h"  // For CameraView

namespace we
{
    struct RenderConfig
    {
        vec2f RenderResolution;
        bool SetRenderSmooth;
    };

    // Render layers - each goes to a different render target
    enum class ERenderLayer : uint8
    {
        World,      // Game world - uses camera view
        ScreenUI,   // Screen-space UI - uses default view
        WorldUI,    // World-space UI - uses camera view
        Cursor      // Cursor - uses default view
    };

    // Which view space a drawable should be rendered in
    enum class EViewSpace
    {
        World,   // Uses camera view (for gameplay objects, world UI)
        Screen   // Uses default view (for screen UI, cursor)
    };

    class RenderSubsystem
    {
    public:
        explicit RenderSubsystem(const RenderConfig& Config);

        // Draw an object to a specific layer with specified view space
        void Draw(const drawable& RenderObject, ERenderLayer Layer, EViewSpace ViewSpace);

        // Convenience: Draw with default view space for the layer
        void Draw(const drawable& RenderObject, ERenderLayer Layer);

        // Frame management
        void BeginFrame();
        void SetWorldView(const CameraView& Camera);
        void ResetToDefaultViews();
        void ResetWorldViewToDefault();  // Reset world targets to default view

        // Returns the final composite sprite
        sprite FinishRender();

    private:
        // World Render (gameplay objects)
        renderTexture WorldRenderTarget;
        renderTexture WorldPostProcessTarget;
        vector<unique<IPostProcess>> WorldPostProcessEffects;

        // Screen UI Render (HUD, menus - screen space)
        renderTexture ScreenUIRenderTarget;
        renderTexture ScreenUIPostProcessTarget;
        vector<unique<IPostProcess>> ScreenUIPostEffects;

        // World UI Render (health bars, popups - world space)
        renderTexture WorldUIRenderTarget;
        renderTexture WorldUIPostProcessTarget;
        vector<unique<IPostProcess>> WorldUIPostProcessEffects;

        // Cursor Render
        renderTexture CursorRenderTarget;
        renderTexture CursorPostProcessTarget;
        vector<unique<IPostProcess>> CursorPostProcessEffects;

        // Final composite target
        renderTexture CompositeTarget;

    private:
        friend class WaterEngine;

        void CompositeLayers();
        renderTexture* ProcessPostEffects(renderTexture* Input, renderTexture* Output, vector<unique<IPostProcess>>& Effects);
        renderTexture* GetTargetForLayer(ERenderLayer Layer);
        void ApplyViewToTarget(renderTexture& Target, EViewSpace ViewSpace, const view& WorldView, const view& DefaultView);

    private:
        vec2u RenderResolution;
        RenderConfig Config;
        
        // Cached views
        view CurrentWorldView;
        view CurrentDefaultView;
    };
}

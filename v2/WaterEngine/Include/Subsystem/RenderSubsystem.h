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
    class WindowSubsystem;

    struct RenderConfig
    {
        vec2f RenderResolution;
        bool  SetRenderSmooth;
    };

    // Render layers - each renders to a different target
    enum class ERenderLayer : uint8
    {
        World,      // Game world - uses camera view
        ScreenUI,   // Screen-space UI - uses default view
        WorldUI,    // World-space UI - uses camera view
        Cursor      // Cursor - uses default view, 1:1 with window
    };

    // Which view space a drawable should be rendered in
    enum class EViewSpace
    {
        World,   // Uses camera view (gameplay objects, world UI)
        Screen   // Uses default view (screen UI, cursor)
    };

    class RenderSubsystem
    {
    public:
        explicit RenderSubsystem(WindowSubsystem& Window, const RenderConfig& Config);

        // Drawing
        void Draw(const drawable& RenderObject, ERenderLayer Layer);
        void Draw(const drawable& RenderObject, ERenderLayer Layer, EViewSpace ViewSpace);

        // Frame management
        void BeginFrame();
        void SetWorldView(const CameraView& Camera);
        void ResetToDefaultViews();
        void ResetWorldViewToDefault();

        // Final composition and presentation
        sprite FinishComposite();   // Returns game+UI sprite (letterboxed)
        void PresentCursor();       // Draws cursor 1:1 to window
        
        // Coordinate mapping from window pixels to game coordinates
        vec2f MapPixelToCoords(const vec2i& PixelPos);

    private:
        // Render targets for each layer
        renderTexture WorldRenderTarget;
        renderTexture WorldPostProcessTarget;
        vector<unique<IPostProcess>> WorldPostProcessEffects;

        renderTexture ScreenUIRenderTarget;
        renderTexture ScreenUIPostProcessTarget;
        vector<unique<IPostProcess>> ScreenUIPostEffects;

        renderTexture WorldUIRenderTarget;
        renderTexture WorldUIPostProcessTarget;
        vector<unique<IPostProcess>> WorldUIPostProcessEffects;

        renderTexture CursorRenderTarget;
        renderTexture CursorPostProcessTarget;
        vector<unique<IPostProcess>> CursorPostProcessEffects;

        // Final composite target
        renderTexture CompositeTarget;

        // Views
        view CurrentWorldView;  // Cached camera view for world layers
        view LetterboxView;     // View for mapping window coords to game coords

    private:
        // Helper methods
        void ClearRenderTargets();
        void CompositeLayers();
        void UpdateLetterboxView(const vec2u& WindowSize, float Scale, float PosX, float PosY);
        
        renderTexture* GetTargetForLayer(ERenderLayer Layer);
        renderTexture* ProcessPostEffects(renderTexture* Input, renderTexture* Output, vector<unique<IPostProcess>>& Effects);
        const texture& GetLayerTexture(renderTexture& MainTarget, renderTexture& PostProcessTarget, vector<unique<IPostProcess>>& Effects);

    private:
        WindowSubsystem& RenderWindow;
        vec2u            RenderResolution;
        RenderConfig     Config;
    };

}

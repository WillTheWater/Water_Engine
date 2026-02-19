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

    enum class ERenderLayer : uint8
    {
        Game,
        UI,
        Cursor
    };

    class RenderSubsystem
    {
    public:
        explicit RenderSubsystem(const RenderConfig& Config);

        void Draw(const drawable& RenderObject, ERenderLayer Layer = ERenderLayer::Game);

        // Returns the final composite sprite
        sprite FinishRender();

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

        // Final composite target
        renderTexture CompositeTarget;


    private:
        friend class WaterEngine;

        void ApplyRenderView();
        void ApplyCameraView(const optional<CameraView>& View);  // Optional camera view
        void CompositeLayers();
        void Initialize();
        void StartRender();
        renderTexture* ProcessPostEffects(renderTexture* Input, renderTexture* Output, vector<unique<IPostProcess>>& Effects);
        renderTexture* GetTargetForLayer(ERenderLayer Layer);

    private:
        vec2u RenderResolution;
        RenderConfig Config;
    };
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/RenderSubsystem.h"
#include "Utility/Assert.h"
#include "EngineConfig.h"
#include "Utility/Log.h"

// Post-process includes
#include "PostProcess/PPE/BloomPPE.h"

namespace we
{
    RenderSubsystem::RenderSubsystem(const RenderConfig& InConfig)
        : RenderResolution(static_cast<vec2u>(InConfig.RenderResolution))
        , Config{ InConfig }
    {
        // Set Render Targets Pixel Size
        VERIFY(WorldRenderTarget.resize(RenderResolution));
        VERIFY(ScreenUIRenderTarget.resize(RenderResolution));
        VERIFY(WorldUIRenderTarget.resize(RenderResolution));
        VERIFY(CursorRenderTarget.resize(RenderResolution));
        VERIFY(CompositeTarget.resize(RenderResolution));

        // Set Smoothing for Render Targets
        WorldRenderTarget.setSmooth(Config.SetRenderSmooth);
        ScreenUIRenderTarget.setSmooth(false);
        WorldUIRenderTarget.setSmooth(Config.SetRenderSmooth);
        CursorRenderTarget.setSmooth(false);
        CompositeTarget.setSmooth(false);

        // Apply Shaders to Render Targets
        if (sf::Shader::isAvailable())
        {
            VERIFY(WorldPostProcessTarget.resize(RenderResolution));
            VERIFY(ScreenUIPostProcessTarget.resize(RenderResolution));
            VERIFY(WorldUIPostProcessTarget.resize(RenderResolution));
            VERIFY(CursorPostProcessTarget.resize(RenderResolution));

            // PPE Applied to World (gameplay)
            WorldPostProcessEffects.emplace_back(make_unique<BloomPPE>());
        }

        // Cache default view
        CurrentDefaultView = WorldRenderTarget.getDefaultView();
    }

    renderTexture* RenderSubsystem::GetTargetForLayer(ERenderLayer Layer)
    {
        switch (Layer)
        {
            case ERenderLayer::World:      return &WorldRenderTarget;
            case ERenderLayer::ScreenUI:   return &ScreenUIRenderTarget;
            case ERenderLayer::WorldUI:    return &WorldUIRenderTarget;
            case ERenderLayer::Cursor:     return &CursorRenderTarget;
            default:                       return &WorldRenderTarget;
        }
    }

    void RenderSubsystem::Draw(const drawable& RenderObject, ERenderLayer Layer, EViewSpace ViewSpace)
    {
        renderTexture* Target = GetTargetForLayer(Layer);

        // Apply appropriate view before drawing
        if (ViewSpace == EViewSpace::World)
        {
            Target->setView(CurrentWorldView);
        }
        else
        {
            Target->setView(CurrentDefaultView);
        }

        Target->draw(RenderObject);
    }

    void RenderSubsystem::Draw(const drawable& RenderObject, ERenderLayer Layer)
    {
        // Default view space based on layer
        EViewSpace DefaultSpace = EViewSpace::Screen;
        switch (Layer)
        {
            case ERenderLayer::World:
            case ERenderLayer::WorldUI:
                DefaultSpace = EViewSpace::World;
                break;
            case ERenderLayer::ScreenUI:
            case ERenderLayer::Cursor:
                DefaultSpace = EViewSpace::Screen;
                break;
        }
        Draw(RenderObject, Layer, DefaultSpace);
    }

    void RenderSubsystem::BeginFrame()
    {
        // Clear all layer targets
        WorldRenderTarget.clear(color{ 86, 164, 183 });
        ScreenUIRenderTarget.clear(color::Transparent);
        WorldUIRenderTarget.clear(color::Transparent);
        CursorRenderTarget.clear(color::Transparent);
        CompositeTarget.clear(color::Transparent);

        if (sf::Shader::isAvailable())
        {
            WorldPostProcessTarget.clear(color::Transparent);
            ScreenUIPostProcessTarget.clear(color::Transparent);
            WorldUIPostProcessTarget.clear(color::Transparent);
            CursorPostProcessTarget.clear(color::Transparent);
        }

        // Reset to default views initially
        ResetToDefaultViews();
    }

    void RenderSubsystem::SetWorldView(const CameraView& Camera)
    {
        // Calculate aspect ratio from EngineConfig (e.g., 16.0f / 9.0f)
        float AspectRatio = EC.AspectRatio.x / EC.AspectRatio.y;
        
        // Get view size based on camera orthographic size and aspect ratio
        vec2f ViewSize = Camera.GetViewSize(AspectRatio);

        view WorldView;
        WorldView.setCenter(Camera.Position);
        WorldView.setSize(ViewSize);
        WorldView.setRotation(sf::radians(Camera.Rotation));
        WorldView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));

        // Cache the world view
        CurrentWorldView = WorldView;

        // Apply to world targets immediately
        WorldRenderTarget.setView(WorldView);
        WorldPostProcessTarget.setView(WorldView);
        WorldUIRenderTarget.setView(WorldView);
        WorldUIPostProcessTarget.setView(WorldView);
    }

    void RenderSubsystem::ResetToDefaultViews()
    {
        CurrentDefaultView = WorldRenderTarget.getDefaultView();
        CurrentDefaultView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));

        // Screen UI and Cursor use default view
        ScreenUIRenderTarget.setView(CurrentDefaultView);
        ScreenUIPostProcessTarget.setView(CurrentDefaultView);
        CursorRenderTarget.setView(CurrentDefaultView);
        CursorPostProcessTarget.setView(CurrentDefaultView);
    }

    renderTexture* RenderSubsystem::ProcessPostEffects(renderTexture* Input, renderTexture* Output, vector<unique<IPostProcess>>& Effects)
    {
        if (Effects.empty())
        {
            Input->display();
            return Input;
        }

        Input->display();
        renderTexture* In = Input;
        renderTexture* Out = Output;

        for (auto& Effect : Effects)
        {
            Out->clear(color::Transparent);
            Effect->Apply(In->getTexture(), *Out);
            Out->display();
            std::swap(In, Out);
        }

        return In;
    }

    void RenderSubsystem::CompositeLayers()
    {
        // Render Post Processing for each layer
        auto RenderLayer = [&](renderTexture& main, renderTexture& pp, vector<unique<IPostProcess>>& effects) -> const texture& {
            if (sf::Shader::isAvailable() && !effects.empty()) {
                return ProcessPostEffects(&main, &pp, effects)->getTexture();
            }
            main.display();
            return main.getTexture();
        };

        const texture& RenderedWorldLayer = RenderLayer(WorldRenderTarget, WorldPostProcessTarget, WorldPostProcessEffects);
        const texture& RenderedScreenUILayer = RenderLayer(ScreenUIRenderTarget, ScreenUIPostProcessTarget, ScreenUIPostEffects);
        const texture& RenderedWorldUILayer = RenderLayer(WorldUIRenderTarget, WorldUIPostProcessTarget, WorldUIPostProcessEffects);
        const texture& RenderedCursorLayer = RenderLayer(CursorRenderTarget, CursorPostProcessTarget, CursorPostProcessEffects);

        // Composite all layers
        CompositeTarget.clear(color::Transparent);
        CompositeTarget.setView(CompositeTarget.getDefaultView());

        // Draw in order: World -> WorldUI -> ScreenUI -> Cursor
        CompositeTarget.draw(sprite(RenderedWorldLayer));
        CompositeTarget.draw(sprite(RenderedWorldUILayer), sf::BlendAlpha);
        CompositeTarget.draw(sprite(RenderedScreenUILayer), sf::BlendAlpha);
        CompositeTarget.draw(sprite(RenderedCursorLayer), sf::BlendAlpha);

        CompositeTarget.display();
    }

    sprite RenderSubsystem::FinishRender()
    {
        CompositeLayers();
        return sprite(CompositeTarget.getTexture());
    }
}

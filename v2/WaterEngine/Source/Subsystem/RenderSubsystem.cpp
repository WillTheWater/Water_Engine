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
        VERIFY(GameRenderTarget.resize(RenderResolution));
        VERIFY(UIRenderTarget.resize(RenderResolution));
        VERIFY(CursorRenderTarget.resize(RenderResolution));
        VERIFY(CompositeTarget.resize(RenderResolution));

        // Set Smoothing for Render Targets
        GameRenderTarget.setSmooth(Config.SetRenderSmooth);
        UIRenderTarget.setSmooth(false);
        CursorRenderTarget.setSmooth(false);
        CompositeTarget.setSmooth(false);

        // Apply Shaders to Render Targets
        if (sf::Shader::isAvailable())
        {
            VERIFY(GamePostProcessTarget.resize(RenderResolution));
            VERIFY(UIPostProcessTarget.resize(RenderResolution));
            VERIFY(CursorPostProcessTarget.resize(RenderResolution));

            //PPE Applied to Game
            GamePostProcessEffects.emplace_back(make_unique<BloomPPE>());
        }
    }

    renderTexture* RenderSubsystem::GetTargetForLayer(ERenderLayer Layer)
    {
        switch (Layer)
        {
            case ERenderLayer::Game:    return &GameRenderTarget;
            case ERenderLayer::UI:      return &UIRenderTarget;
            case ERenderLayer::Cursor:  return &CursorRenderTarget;
            default:                    return &GameRenderTarget;
        }
    }

    void RenderSubsystem::Draw(const drawable& RenderObject, ERenderLayer Layer)
    {
        GetTargetForLayer(Layer)->draw(RenderObject);
    }

    void RenderSubsystem::ApplyRenderView()
    {
        view DefaultView = GameRenderTarget.getDefaultView();
        DefaultView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));

        GameRenderTarget.setView(DefaultView);
        GamePostProcessTarget.setView(DefaultView);
        UIRenderTarget.setView(DefaultView);
        UIPostProcessTarget.setView(DefaultView);
        CursorRenderTarget.setView(DefaultView);
        CursorPostProcessTarget.setView(DefaultView);
    }

    void RenderSubsystem::ApplyCameraView(const optional<CameraView>& View)
    {
        // Use camera view if provided, otherwise use default
        if (View)
        {
            // Calculate view from camera
            float AspectRatio = static_cast<float>(RenderResolution.x) / static_cast<float>(RenderResolution.y);
            vec2f ViewSize = View->GetViewSize(AspectRatio);
            
            view SFMLView;
            SFMLView.setCenter(View->Position);
            SFMLView.setSize(ViewSize);
            SFMLView.setRotation(sf::radians(View->Rotation));
            SFMLView.setViewport(rectf({0.f, 0.f}, {1.f, 1.f}));
            
            // Apply to game render targets only (UI and Cursor stay screen-space)
            GameRenderTarget.setView(SFMLView);
            GamePostProcessTarget.setView(SFMLView);
            LOG("Using Camera");
        }
        else
        {
            // No camera - use default view (existing behavior)
            view DefaultView = GameRenderTarget.getDefaultView();
            DefaultView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));
            
            GameRenderTarget.setView(DefaultView);
            GamePostProcessTarget.setView(DefaultView);
        }
        
        // UI and Cursor always use default (screen-space)
        view DefaultView = GameRenderTarget.getDefaultView();
        DefaultView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));
        UIRenderTarget.setView(DefaultView);
        UIPostProcessTarget.setView(DefaultView);
        CursorRenderTarget.setView(DefaultView);
        CursorPostProcessTarget.setView(DefaultView);
    }

    void RenderSubsystem::StartRender()
    {
        // Clear all layer targets
        GameRenderTarget.clear(color{ 86, 164, 183 });
        UIRenderTarget.clear(color::Transparent);
        CursorRenderTarget.clear(color::Transparent);
        CompositeTarget.clear(color::Transparent);

        if (sf::Shader::isAvailable)
        {
            GamePostProcessTarget.clear(color::Transparent);
            UIPostProcessTarget.clear(color::Transparent);
            CursorPostProcessTarget.clear(color::Transparent);
        }
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
        // Render Post Processing
        auto RenderLayer = [&](renderTexture& main, renderTexture& pp, vector<unique<IPostProcess>>& effects) -> const texture& {
            if (sf::Shader::isAvailable && !effects.empty()) {
                return ProcessPostEffects(&main, &pp, effects)->getTexture();
            }
            main.display();
            return main.getTexture();
            };

        const texture& RenderedGameLayer = RenderLayer(GameRenderTarget, GamePostProcessTarget, GamePostProcessEffects);
        const texture& RenderedUILayer = RenderLayer(UIRenderTarget, UIPostProcessTarget, UIPostEffects);
        const texture& RenderedCursorLayer = RenderLayer(CursorRenderTarget, CursorPostProcessTarget, CursorPostProcessEffects);

        // Composite
        CompositeTarget.clear(color::Transparent);
        CompositeTarget.setView(CompositeTarget.getDefaultView());

        CompositeTarget.draw(sprite(RenderedGameLayer));
        CompositeTarget.draw(sprite(RenderedUILayer), sf::BlendAlpha);
        CompositeTarget.draw(sprite(RenderedCursorLayer), sf::BlendAlpha);

        CompositeTarget.display();
    }

    sprite RenderSubsystem::FinishRender()
    {
        CompositeLayers();
        return sprite(CompositeTarget.getTexture());
    }
}
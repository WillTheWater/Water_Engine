// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/RenderSubsystem.h"
#include "Core/EngineConfig.h"
#include "Utility/Assert.h"
#include "Utility/Log.h"

#include <cmath>

namespace we
{
    RenderSubsystem::RenderSubsystem()
        : RenderResolution{WEConfig.Render.RenderResolution}
        , bNeedsComposite{false}
    {
        CreateRenderTargets();
    }

    void RenderSubsystem::SetTargetSize(vec2u Size)
    {
        if (RenderResolution != Size)
        {
            RenderResolution = Size;
            CreateRenderTargets();
        }
    }

    void RenderSubsystem::CreateRenderTargets()
    {
        VERIFY(WorldRenderTarget.resize(RenderResolution));
        VERIFY(WorldUIRenderTarget.resize(RenderResolution));
        VERIFY(ScreenUIRenderTarget.resize(RenderResolution));
        VERIFY(CursorRenderTarget.resize(RenderResolution));
        VERIFY(Composite.resize(RenderResolution));

        WorldRenderTarget.setSmooth(WEConfig.Render.bWorldLayerSmooth);
        ScreenUIRenderTarget.setSmooth(WEConfig.Render.bScreenUILayerSmooth);
        WorldUIRenderTarget.setSmooth(WEConfig.Render.bWorldUILayerSmooth);
        CursorRenderTarget.setSmooth(WEConfig.Render.bCursorLayerSmooth);
        Composite.setSmooth(WEConfig.Render.bCompositeSmooth);

        if (shader::isAvailable)
        {
            VERIFY(WorldPostProcessTarget.resize(RenderResolution));
            VERIFY(CompositePostProcessTarget.resize(RenderResolution));
        }
    }

    void RenderSubsystem::SetWorldView(vec2f Center, float Zoom, float Rotation)
    {
        vec2f ViewSize = vec2f(RenderResolution) / Zoom;

        // Snap the center to whole device pixels (1 world unit = Zoom px) so the
        // world doesn't render on fractional pixels as the camera moves.
        const vec2f SnappedCenter = { std::round(Center.x * Zoom) / Zoom,
                                      std::round(Center.y * Zoom) / Zoom };

        view WorldView;
        WorldView.setCenter(SnappedCenter);
        WorldView.setSize(ViewSize);
        WorldView.setRotation(sf::radians(Rotation));
        
        WorldRenderTarget.setView(WorldView);
        if (shader::isAvailable)
        {
            WorldPostProcessTarget.setView(WorldView);
        }
    }

    void RenderSubsystem::BeginFrame()
    {
        ClearRenderTargets();
        bNeedsComposite = true;
    }

    void RenderSubsystem::Draw(const drawable& RenderObject, ERenderLayer Layer)
    {
        switch (Layer)
        {
            case ERenderLayer::World:
                WorldRenderTarget.draw(RenderObject);
                break;
            case ERenderLayer::WorldUI:
                WorldUIRenderTarget.draw(RenderObject);
                break;
            case ERenderLayer::ScreenUI:
                ScreenUIRenderTarget.draw(RenderObject);
                break;
            case ERenderLayer::Cursor:
                CursorRenderTarget.draw(RenderObject);
                break;
        }
    }

    void RenderSubsystem::EndFrame()
    {
        if (bNeedsComposite)
        {
            CompositeLayers();
            bNeedsComposite = false;
        }
    }

    sprite RenderSubsystem::GetCompositeSprite() const
    {
        return sprite(Composite.getTexture());
    }

    void RenderSubsystem::ClearRenderTargets()
    {
        WorldRenderTarget.clear(WEConfig.Render.WindowClearColor);
        ScreenUIRenderTarget.clear(WEConfig.Render.ScreenUIClearColor);
        WorldUIRenderTarget.clear(WEConfig.Render.WorldUIClearColor);
        CursorRenderTarget.clear(WEConfig.Render.CursorClearColor);
        Composite.clear(WEConfig.Render.CompositeClearColor);

        if (shader::isAvailable)
        {
            WorldPostProcessTarget.clear(color::Transparent);
            CompositePostProcessTarget.clear(color::Transparent);
        }
    }

    void RenderSubsystem::AddWorldPostProcessEffect(unique<IPostProcess> Effect)
    {
        if (Effect)
            WorldPostProcessEffects.push_back(std::move(Effect));
    }

    void RenderSubsystem::ClearWorldPostProcessEffects()
    {
        WorldPostProcessEffects.clear();
    }

    void RenderSubsystem::PostProcess(renderTexture* Input, renderTexture* Output, vector<unique<IPostProcess>>& Effects)
    {
        if (Effects.empty())
        {
            Input->display();
            return;
        }

        renderTexture* In = Input;
        renderTexture* Out = Output;

        for (auto& Effect : Effects)
        {
            Out->clear(color::Transparent);
            Effect->Apply(In->getTexture(), *Out);
            Out->display();
            std::swap(In, Out);
        }

        if (In != Input)
        {
            // Pixel-exact blit: the world target carries the camera view, which
            // would transform a screen-sized sprite. Use the default view for
            // the copy-back, then restore.
            const view SavedView = Input->getView();
            Input->setView(Input->getDefaultView());
            Input->clear(color::Transparent);
            sprite FinalSprite(In->getTexture());
            Input->draw(FinalSprite);
            Input->setView(SavedView);
            Input->display();
        }
        else
        {
            Input->display();
        }
    }


    void RenderSubsystem::ApplyPostProcess(renderTexture& MainTarget, renderTexture& PostProcessTarget, vector<unique<IPostProcess>>& Effects)
    {
        if (shader::isAvailable)
        {
            PostProcess(&MainTarget, &PostProcessTarget, Effects);
        }

        MainTarget.display();
    }

    void RenderSubsystem::CompositeLayers()
    {
        ApplyPostProcess(WorldRenderTarget, WorldPostProcessTarget, WorldPostProcessEffects);

        WorldUIRenderTarget.display();
        ScreenUIRenderTarget.display();
        CursorRenderTarget.display();

        sprite WorldSprite(WorldRenderTarget.getTexture());
        Composite.draw(WorldSprite);

        sprite WorldUISprite(WorldUIRenderTarget.getTexture());
        Composite.draw(WorldUISprite, sf::BlendAlpha);

        sprite ScreenUISprite(ScreenUIRenderTarget.getTexture());
        Composite.draw(ScreenUISprite, sf::BlendAlpha);

        sprite CursorSprite(CursorRenderTarget.getTexture());
        Composite.draw(CursorSprite, sf::BlendAlpha);

        ApplyPostProcess(Composite, CompositePostProcessTarget, CompositePostProcessEffects);
    }
}
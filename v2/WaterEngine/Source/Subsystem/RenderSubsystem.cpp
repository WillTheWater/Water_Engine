// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/RenderSubsystem.h"
#include "Utility/Assert.h"
#include "EngineConfig.h"

// Post-process includes
#include "PostProcess/PPE/BloomPPE.h"

namespace we
{
    RenderSubsystem::RenderSubsystem()
        : CurrentWindowSize(EC.WindowSize)
    {
        Initialize();
    }

    void RenderSubsystem::Initialize()
    {
        // Set Render Targets Pixel Size
        VERIFY(GameRenderTarget.resize(vec2u(EC.WindowSize)));
        VERIFY(UIRenderTarget.resize(vec2u(EC.WindowSize)));
        VERIFY(CursorRenderTarget.resize(vec2u(EC.WindowSize)));
        VERIFY(CompositeTarget.resize(vec2u(EC.WindowSize)));

        // Set Smoothing for Render Targets
        GameRenderTarget.setSmooth(EC.SetRenderSmooth);
        UIRenderTarget.setSmooth(EC.SetRenderSmooth);
        CursorRenderTarget.setSmooth(EC.SetRenderSmooth);
        CompositeTarget.setSmooth(EC.SetRenderSmooth);

        // *********************************************************
         //
         //				TODO: Make this Editable-Dynamic
         //
         // *********************************************************

         // Apply Shaders to Render Targets
        if (sf::Shader::isAvailable)
        {
            VERIFY(GamePostProcessTarget.resize(vec2u(EC.WindowSize)));
            VERIFY(UIPostProcessTarget.resize(vec2u(EC.WindowSize)));
            VERIFY(CursorPostProcessTarget.resize(vec2u(EC.WindowSize)));

            //PPE Applied to Game
            GamePostProcessEffects.emplace_back(make_unique<BloomPPE>());

            //PPE Applied to UI
            // UIPostEffects.emplace_back(make_unique<PPETest>()); Example

            //PPE Applied to Cursor (Rare)
            // CursorPostProcessEffects.emplace_back(make_unique<TimePPETest>()); Example
        }
    }

    void RenderSubsystem::OnWindowResized(vec2u NewWindowSize)
    {
        CurrentWindowSize = NewWindowSize;
        // Optionally resize targets if you want dynamic resolution
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

    void RenderSubsystem::SetRenderView(const view& RenderView)
    {
        GameRenderTarget.setView(RenderView);
        GamePostProcessTarget.setView(RenderView);
    }

    void RenderSubsystem::SetLayerView(ERenderLayer Layer, const view& View)
    {
        renderTexture* Target = GetTargetForLayer(Layer);
        if (Target)
        {
            view TextureView = View;
            TextureView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));
            Target->setView(TextureView);
        }
    }

    void RenderSubsystem::ResetRenderView()
    {
        SetRenderView(GameRenderTarget.getDefaultView());
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

    vec2f RenderSubsystem::WindowToRenderCoords(vec2i WindowPixel, const view& View) const
    {
        // Requires the view to be provided by caller (from window)
        // This is now window-agnostic
        return vec2f(View.getCenter().x - View.getSize().x / 2.0f +
            (WindowPixel.x / static_cast<float>(CurrentWindowSize.x)) * View.getSize().x,
            View.getCenter().y - View.getSize().y / 2.0f +
            (WindowPixel.y / static_cast<float>(CurrentWindowSize.y)) * View.getSize().y);
    }

    vec2i RenderSubsystem::RenderToWindowCoords(vec2f RenderPos, const view& View) const
    {
        return vec2i(
            static_cast<int>((RenderPos.x - View.getCenter().x + View.getSize().x / 2.0f) /
                View.getSize().x * CurrentWindowSize.x),
            static_cast<int>((RenderPos.y - View.getCenter().y + View.getSize().y / 2.0f) /
                View.getSize().y * CurrentWindowSize.y)
        );
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

    void RenderSubsystem::ProcessPostEffects(renderTexture* Input, renderTexture* Output, vector<unique<IPostProcess>>& Effects)
    {
        if (Effects.empty())
        {
            Input->display();
            return;
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
    }

    void RenderSubsystem::CompositeLayers()
    {
        // Get final textures for each layer
        const texture* GameTexture = &GameRenderTarget.getTexture();
        const texture* UITexture = &UIRenderTarget.getTexture();
        const texture* CursorTexture = &CursorRenderTarget.getTexture();

        // Apply post-processing
        if (sf::Shader::isAvailable)
        {
            if (!GamePostProcessEffects.empty())
            {
                ProcessPostEffects(&GameRenderTarget, &GamePostProcessTarget, GamePostProcessEffects);
                GameTexture = &GamePostProcessTarget.getTexture();
            }
            else
            {
                GameRenderTarget.display();
            }

            if (!UIPostEffects.empty())
            {
                ProcessPostEffects(&UIRenderTarget, &UIPostProcessTarget, UIPostEffects);
                UITexture = &UIPostProcessTarget.getTexture();
            }
            else
            {
                UIRenderTarget.display();
            }

            if (!CursorPostProcessEffects.empty())
            {
                ProcessPostEffects(&CursorRenderTarget, &CursorPostProcessTarget, CursorPostProcessEffects);
                CursorTexture = &CursorPostProcessTarget.getTexture();
            }
            else
            {
                CursorRenderTarget.display();
            }
        }
        else
        {
            GameRenderTarget.display();
            UIRenderTarget.display();
            CursorRenderTarget.display();
        }

        // Composite all layers to internal CompositeTarget (no window dependency!)
        CompositeTarget.clear(color::Transparent);

        view DefaultView = CompositeTarget.getDefaultView();
        CompositeTarget.setView(DefaultView);

        // Layer 1: Game (background)
        sprite GameSprite(*GameTexture);
        CompositeTarget.draw(GameSprite);

        // Layer 2: UI (middle)
        sprite UISprite(*UITexture);
        CompositeTarget.draw(UISprite, sf::BlendAlpha);

        // Layer 3: Cursor (top)
        sprite CursorSprite(*CursorTexture);
        CompositeTarget.draw(CursorSprite, sf::BlendAlpha);

        CompositeTarget.display();
    }

    const texture& RenderSubsystem::FinishRender()
    {
        CompositeLayers();
        return CompositeTarget.getTexture();
    }
}
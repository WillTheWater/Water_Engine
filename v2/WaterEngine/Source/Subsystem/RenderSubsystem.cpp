// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Utility/Assert.h"
#include "EngineConfig.h"
#include "Utility/Log.h"

// Post-process includes
#include "PostProcess/PPE/BloomPPE.h"

namespace we
{
    RenderSubsystem::RenderSubsystem(WindowSubsystem& Window, const RenderConfig& InConfig)
        : RenderWindow{Window}
        , RenderResolution(static_cast<vec2u>(InConfig.RenderResolution))
        , Config{ InConfig }
    {
        // Set Render Targets Pixel Size
        VERIFY(WorldRenderTarget.resize(RenderResolution));
        VERIFY(ScreenUIRenderTarget.resize(RenderResolution));
        VERIFY(WorldUIRenderTarget.resize(RenderResolution));
        VERIFY(CompositeTarget.resize(RenderResolution));
        VERIFY(CursorRenderTarget.resize(Window.getSize()));

        // Set Smoothing for Render Targets
        WorldRenderTarget.setSmooth(Config.SetRenderSmooth);
        ScreenUIRenderTarget.setSmooth(false);
        WorldUIRenderTarget.setSmooth(false);
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

        // Special handling for Cursor: It always uses a 1:1 view of its target size
        if (Layer == ERenderLayer::Cursor)
        {
            view CursorView = CursorRenderTarget.getDefaultView();
            CursorRenderTarget.setView(CursorView);
        }
        else
        {
            if (ViewSpace == EViewSpace::World)
                Target->setView(CurrentWorldView);
            else
                Target->setView(Target->getDefaultView()); // Default view of the fixed target
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
        // 1. Handle Cursor Target Resizing (Must match Window, not Game Resolution)
        vec2u WindowSize = RenderWindow.getSize();
        if (CursorRenderTarget.getSize() != WindowSize)
        {
            CursorRenderTarget.resize(WindowSize);
        }

        // 2. Clear Fixed Targets
        WorldRenderTarget.clear(color{ 86, 164, 183 });
        ScreenUIRenderTarget.clear(color::Transparent);
        WorldUIRenderTarget.clear(color::Transparent);
        CompositeTarget.clear(color::Transparent);

        if (sf::Shader::isAvailable)
        {
            WorldPostProcessTarget.clear(color::Transparent);
            ScreenUIPostProcessTarget.clear(color::Transparent);
            WorldUIPostProcessTarget.clear(color::Transparent);
            CursorPostProcessTarget.clear(color::Transparent);
        }

        // 3. Clear Cursor Target
        CursorRenderTarget.clear(color::Transparent);

        // 4. Reset Views
        ResetWorldViewToDefault();
        ResetToDefaultViews();
    }

    void RenderSubsystem::ResetWorldViewToDefault()
    {
        // Reset world targets to default view (when no camera is active)
        view DefaultView = WorldRenderTarget.getDefaultView();
        DefaultView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));
        
        WorldRenderTarget.setView(DefaultView);
        WorldPostProcessTarget.setView(DefaultView);
        WorldUIRenderTarget.setView(DefaultView);
        WorldUIPostProcessTarget.setView(DefaultView);
        
        CurrentWorldView = DefaultView;
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
        auto CurrentDefaultView = WorldRenderTarget.getDefaultView();
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

        // Display all inputs
        WorldRenderTarget.display();
        ScreenUIRenderTarget.display();
        WorldUIRenderTarget.display();
        // NOTE: We do NOT display CursorRenderTarget here. It is handled separately.

        // Composite Game Layers Only
        CompositeTarget.setView(CompositeTarget.getDefaultView());

        // World
        sprite WorldSprite(WorldRenderTarget.getTexture());
        CompositeTarget.draw(WorldSprite);

        // World UI
        sprite WorldUISprite(WorldUIRenderTarget.getTexture());
        CompositeTarget.draw(WorldUISprite, sf::BlendAlpha);

        // Screen UI
        sprite ScreenUISprite(ScreenUIRenderTarget.getTexture());
        CompositeTarget.draw(ScreenUISprite, sf::BlendAlpha);

        CompositeTarget.display();
    }

    sprite RenderSubsystem::FinishComposite()
    {
        CompositeLayers();

        sprite FinalSprite(CompositeTarget.getTexture());

        // Calculate Letterbox Scale
        vec2u WindowSize = RenderWindow.getSize();
        float ScaleX = static_cast<float>(WindowSize.x) / RenderResolution.x;
        float ScaleY = static_cast<float>(WindowSize.y) / RenderResolution.y;

        // Maintain Aspect Ratio (Fit Inside)
        float Scale = std::min(ScaleX, ScaleY);
        FinalSprite.setScale({ Scale, Scale });

        // Center it
        float PosX = (WindowSize.x - (RenderResolution.x * Scale)) / 2.f;
        float PosY = (WindowSize.y - (RenderResolution.y * Scale)) / 2.f;
        FinalSprite.setPosition({ PosX, PosY });

        // Cache the view for coordinate mapping
        LetterboxView.setSize(vec2f(RenderResolution));
        LetterboxView.setCenter(vec2f(RenderResolution) / 2.f);
        LetterboxView.setViewport({ { PosX / WindowSize.x, PosY / WindowSize.y }, { (RenderResolution.x * Scale) / WindowSize.x, (RenderResolution.y * Scale) / WindowSize.y } });

        return FinalSprite;
    }

    void RenderSubsystem::PresentCursor()
    {
        CursorRenderTarget.display();

        // Draw cursor 1:1 to window
        view WindowView(sf::FloatRect({ 0.f, 0.f }, vec2f(RenderWindow.getSize())));
        RenderWindow.setView(WindowView);

        sprite CursorSprite(CursorRenderTarget.getTexture());
        RenderWindow.draw(CursorSprite, sf::BlendAlpha);
    }

    vec2f RenderSubsystem::MapPixelToCoords(const vec2i& PixelPos)
    {
        return RenderWindow.mapPixelToCoords(PixelPos, LetterboxView);
    }
}

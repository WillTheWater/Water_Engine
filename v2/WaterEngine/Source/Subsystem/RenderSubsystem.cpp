// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Utility/Assert.h"
#include "EngineConfig.h"
#include "Utility/Log.h"

#include "PostProcess/PPE/BloomPPE.h"

namespace we
{
    // =========================================================================
    // Construction & Initialization
    // =========================================================================
    RenderSubsystem::RenderSubsystem(WindowSubsystem& Window, const RenderConfig& InConfig)
        : RenderWindow{ Window }
        , RenderResolution(static_cast<vec2u>(InConfig.RenderResolution))
        , Config{ InConfig }
    {
        // Initialize render targets at fixed render resolution
        VERIFY(WorldRenderTarget.resize(RenderResolution));
        VERIFY(ScreenUIRenderTarget.resize(RenderResolution));
        VERIFY(WorldUIRenderTarget.resize(RenderResolution));
        VERIFY(CompositeTarget.resize(RenderResolution));
        VERIFY(CursorRenderTarget.resize(Window.getSize()));

        // Configure smoothing
        WorldRenderTarget.setSmooth(Config.SetRenderSmooth);
        ScreenUIRenderTarget.setSmooth(false);
        WorldUIRenderTarget.setSmooth(false);
        CursorRenderTarget.setSmooth(false);
        CompositeTarget.setSmooth(false);

        // Initialize post-processing if shaders are available
        if (sf::Shader::isAvailable())
        {
            VERIFY(WorldPostProcessTarget.resize(RenderResolution));
            VERIFY(ScreenUIPostProcessTarget.resize(RenderResolution));
            VERIFY(WorldUIPostProcessTarget.resize(RenderResolution));
            VERIFY(CursorPostProcessTarget.resize(RenderResolution));

            // Add default post-process effects
            WorldPostProcessEffects.emplace_back(make_unique<BloomPPE>());
        }
    }

    // =========================================================================
    // Drawing
    // =========================================================================
    void RenderSubsystem::Draw(const drawable& RenderObject, ERenderLayer Layer)
    {
        // Default view space based on layer type
        EViewSpace DefaultSpace = (Layer == ERenderLayer::World || Layer == ERenderLayer::WorldUI) 
            ? EViewSpace::World 
            : EViewSpace::Screen;
        
        Draw(RenderObject, Layer, DefaultSpace);
    }

    void RenderSubsystem::Draw(const drawable& RenderObject, ERenderLayer Layer, EViewSpace ViewSpace)
    {
        renderTexture* Target = GetTargetForLayer(Layer);

        // Cursor always uses a 1:1 view of its target size
        if (Layer == ERenderLayer::Cursor)
        {
            view CursorView = CursorRenderTarget.getDefaultView();
            CursorRenderTarget.setView(CursorView);
        }
        else
        {
            // Apply appropriate view based on view space
            Target->setView((ViewSpace == EViewSpace::World) ? CurrentWorldView : Target->getDefaultView());
        }

        Target->draw(RenderObject);
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

    // =========================================================================
    // Frame Management
    // =========================================================================
    void RenderSubsystem::BeginFrame()
    {
        // Resize cursor target to match window size (cursor renders 1:1 with window)
        vec2u WindowSize = RenderWindow.getSize();
        if (CursorRenderTarget.getSize() != WindowSize)
        {
            CursorRenderTarget.resize(WindowSize);
        }

        // Clear all render targets
        ClearRenderTargets();

        // Reset views to default
        ResetWorldViewToDefault();
        ResetToDefaultViews();
    }

    void RenderSubsystem::ClearRenderTargets()
    {
        // Game world background color
        WorldRenderTarget.clear(color{ 86, 164, 183 });
        
        // UI layers start transparent
        ScreenUIRenderTarget.clear(color::Transparent);
        WorldUIRenderTarget.clear(color::Transparent);
        CompositeTarget.clear(color::Transparent);
        CursorRenderTarget.clear(color::Transparent);

        // Clear post-process targets if shaders are available
        if (sf::Shader::isAvailable())
        {
            WorldPostProcessTarget.clear(color::Transparent);
            ScreenUIPostProcessTarget.clear(color::Transparent);
            WorldUIPostProcessTarget.clear(color::Transparent);
            CursorPostProcessTarget.clear(color::Transparent);
        }
    }

    void RenderSubsystem::SetWorldView(const CameraView& Camera)
    {
        // Calculate aspect ratio from config
        float AspectRatio = EC.AspectRatio.x / EC.AspectRatio.y;
        vec2f ViewSize = Camera.GetViewSize(AspectRatio);

        // Build world view
        view WorldView;
        WorldView.setCenter(Camera.Position);
        WorldView.setSize(ViewSize);
        WorldView.setRotation(sf::radians(Camera.Rotation));
        WorldView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));

        // Cache and apply to all world targets
        CurrentWorldView = WorldView;
        
        WorldRenderTarget.setView(WorldView);
        WorldPostProcessTarget.setView(WorldView);
        WorldUIRenderTarget.setView(WorldView);
        WorldUIPostProcessTarget.setView(WorldView);
    }

    void RenderSubsystem::ResetWorldViewToDefault()
    {
        view DefaultView = WorldRenderTarget.getDefaultView();
        DefaultView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));

        WorldRenderTarget.setView(DefaultView);
        WorldPostProcessTarget.setView(DefaultView);
        WorldUIRenderTarget.setView(DefaultView);
        WorldUIPostProcessTarget.setView(DefaultView);

        CurrentWorldView = DefaultView;
    }

    void RenderSubsystem::ResetToDefaultViews()
    {
        view DefaultView = WorldRenderTarget.getDefaultView();
        DefaultView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));

        // Screen UI and Cursor use default view
        ScreenUIRenderTarget.setView(DefaultView);
        ScreenUIPostProcessTarget.setView(DefaultView);
        CursorRenderTarget.setView(DefaultView);
        CursorPostProcessTarget.setView(DefaultView);
    }

    // =========================================================================
    // Post-Processing
    // =========================================================================
    renderTexture* RenderSubsystem::ProcessPostEffects(
        renderTexture* Input, 
        renderTexture* Output, 
        vector<unique<IPostProcess>>& Effects)
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

    const texture& RenderSubsystem::GetLayerTexture(
        renderTexture& MainTarget, 
        renderTexture& PostProcessTarget, 
        vector<unique<IPostProcess>>& Effects)
    {
        if (sf::Shader::isAvailable() && !Effects.empty()) 
        {
            return ProcessPostEffects(&MainTarget, &PostProcessTarget, Effects)->getTexture();
        }
        
        MainTarget.display();
        return MainTarget.getTexture();
    }

    // =========================================================================
    // Compositing & Presentation
    // =========================================================================
    void RenderSubsystem::CompositeLayers()
    {
        // Get textures for each layer (with post-processing applied if available)
        const texture& WorldTex     = GetLayerTexture(WorldRenderTarget,     WorldPostProcessTarget,     WorldPostProcessEffects);
        const texture& ScreenUITex  = GetLayerTexture(ScreenUIRenderTarget,  ScreenUIPostProcessTarget,  ScreenUIPostEffects);
        const texture& WorldUITex   = GetLayerTexture(WorldUIRenderTarget,   WorldUIPostProcessTarget,   WorldUIPostProcessEffects);
        const texture& CursorTex    = GetLayerTexture(CursorRenderTarget,    CursorPostProcessTarget,    CursorPostProcessEffects);

        // Prepare composite target
        CompositeTarget.clear(color::Transparent);
        CompositeTarget.setView(CompositeTarget.getDefaultView());

        // Display all inputs
        WorldRenderTarget.display();
        ScreenUIRenderTarget.display();
        WorldUIRenderTarget.display();
        // Note: CursorRenderTarget is displayed separately in PresentCursor()

        // Composite game layers (World + World UI + Screen UI)
        sprite WorldSprite(WorldRenderTarget.getTexture());
        CompositeTarget.draw(WorldSprite);

        sprite WorldUISprite(WorldUIRenderTarget.getTexture());
        CompositeTarget.draw(WorldUISprite, sf::BlendAlpha);

        sprite ScreenUISprite(ScreenUIRenderTarget.getTexture());
        CompositeTarget.draw(ScreenUISprite, sf::BlendAlpha);

        CompositeTarget.display();
    }

    sprite RenderSubsystem::FinishComposite()
    {
        CompositeLayers();

        // Calculate letterbox/pillarbox to fit render resolution into window
        vec2u WindowSize = RenderWindow.getSize();
        float ScaleX = static_cast<float>(WindowSize.x) / RenderResolution.x;
        float ScaleY = static_cast<float>(WindowSize.y) / RenderResolution.y;

        // Maintain aspect ratio (fit inside window)
        float Scale = std::min(ScaleX, ScaleY);

        // Center the sprite
        float PosX = (WindowSize.x - (RenderResolution.x * Scale)) / 2.f;
        float PosY = (WindowSize.y - (RenderResolution.y * Scale)) / 2.f;

        // Build final sprite
        sprite FinalSprite(CompositeTarget.getTexture());
        FinalSprite.setScale({ Scale, Scale });
        FinalSprite.setPosition({ PosX, PosY });

        // Cache the letterbox view for coordinate mapping
        UpdateLetterboxView(WindowSize, Scale, PosX, PosY);

        return FinalSprite;
    }

    void RenderSubsystem::UpdateLetterboxView(const vec2u& WindowSize, float Scale, float PosX, float PosY)
    {
        LetterboxView.setSize(vec2f(RenderResolution));
        LetterboxView.setCenter(vec2f(RenderResolution) / 2.f);
        
        LetterboxView.setViewport({ 
            { PosX / WindowSize.x, PosY / WindowSize.y }, 
            { (RenderResolution.x * Scale) / WindowSize.x, (RenderResolution.y * Scale) / WindowSize.y } 
        });
    }

    void RenderSubsystem::PresentCursor()
    {
        CursorRenderTarget.display();

        // Draw cursor 1:1 to window (no scaling)
        view WindowView(sf::FloatRect({ 0.f, 0.f }, vec2f(RenderWindow.getSize())));
        RenderWindow.setView(WindowView);

        sprite CursorSprite(CursorRenderTarget.getTexture());
        RenderWindow.draw(CursorSprite, sf::BlendAlpha);
    }

    // =========================================================================
    // Coordinate Mapping
    // =========================================================================
    vec2f RenderSubsystem::MapPixelToCoords(const vec2i& PixelPos)
    {
        return RenderWindow.mapPixelToCoords(PixelPos, LetterboxView);
    }

} // namespace we

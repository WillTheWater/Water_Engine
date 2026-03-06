// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/RenderSubsystem.h"
#include "Utility/Log.h"

namespace we
{
    RenderSubsystem::RenderSubsystem()
        : RenderResolution{1920,1080}
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
        WorldRenderTarget.resize(RenderResolution);
        WorldUIRenderTarget.resize(RenderResolution);
        ScreenUIRenderTarget.resize(RenderResolution);
        CursorRenderTarget.resize(RenderResolution);
        Composite.resize(RenderResolution);

        WorldRenderTarget.setSmooth(true);
        ScreenUIRenderTarget.setSmooth(false);
        WorldUIRenderTarget.setSmooth(false);
        CursorRenderTarget.setSmooth(false);
        Composite.setSmooth(true);
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
        WorldRenderTarget.clear(color::Green);
        ScreenUIRenderTarget.clear(color::Transparent);
        WorldUIRenderTarget.clear(color::Transparent);
        CursorRenderTarget.clear(color::Transparent);
        Composite.clear(color::Transparent);
    }

    void RenderSubsystem::CompositeLayers()
    {
        WorldRenderTarget.display();
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

        Composite.display();
    }
}

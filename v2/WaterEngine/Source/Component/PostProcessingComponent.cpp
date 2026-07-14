// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/PostProcessingComponent.h"
#include "Framework/World/Actor.h"

namespace we
{
    PostProcessingComponent::PostProcessingComponent(Actor* InOwner)
        : Owner(InOwner)
    {
    }

    void PostProcessingComponent::BeginPlay()
    {
        if (!OriginalTexture || Effects.empty())
        {
            ProcessedTexture = OriginalTexture;
            return;
        }

        ApplyEffects();

        if (bWriteToOwnerSprite && Owner && ProcessedTexture)
        {
            Owner->SetSprite(ProcessedTexture);
        }
    }

    void PostProcessingComponent::Tick(float DeltaTime)
    {
        if (Effects.empty()) return;

        for (auto& Effect : Effects)
        {
            Effect->Update(DeltaTime);
        }
        ApplyEffects();

        if (bWriteToOwnerSprite && Owner && ProcessedTexture)
        {
            Owner->SetSprite(ProcessedTexture);
        }
    }

    void PostProcessingComponent::EndPlay()
    {
    }

    Actor* PostProcessingComponent::GetOwner() const
    {
        return Owner;
    }

    void PostProcessingComponent::SetTexture(shared<texture> Tex)
    {
        OriginalTexture = Tex;
        
        if (OriginalTexture)
        {
            OriginalTexture->setSmooth(true);
            OriginalTexture->generateMipmap();
        }
    }

    void PostProcessingComponent::AddEffect(unique<IPostProcess> Effect)
    {
        Effects.push_back(std::move(Effect));
    }

    void PostProcessingComponent::ClearEffects()
    {
        Effects.clear();
    }

    void PostProcessingComponent::ForceRefresh()
    {
        if (Effects.empty()) return;
        ApplyEffects();
        if (bWriteToOwnerSprite && Owner && ProcessedTexture)
            Owner->SetSprite(ProcessedTexture);
    }

    void PostProcessingComponent::SetSourceRect(optional<recti> FrameRect, int Padding)
    {
        SourceRect    = FrameRect;
        SourcePadding = Padding;
    }

    void PostProcessingComponent::ApplyEffects()
    {
        if (!OriginalTexture) return;

        // Pipeline target sized to the source region (sub-rect + padding when
        // SourceRect is set, otherwise the whole texture).
        const vec2u outSize = SourceRect.has_value()
            ? vec2u{ static_cast<uint32_t>(SourceRect->size.x + SourcePadding * 2),
                     static_cast<uint32_t>(SourceRect->size.y + SourcePadding * 2) }
            : OriginalTexture->getSize();

        renderTexture TempTarget;
        TempTarget.resize(outSize);
        TempTarget.clear(color::Transparent);

        sprite src(*OriginalTexture);
        if (SourceRect.has_value())
        {
            src.setTextureRect(*SourceRect);
            src.setPosition({ static_cast<float>(SourcePadding),
                              static_cast<float>(SourcePadding) });
        }
        TempTarget.draw(src);
        TempTarget.display();

        renderTexture PingPongTarget;
        PingPongTarget.resize(outSize);

        renderTexture* In  = &TempTarget;
        renderTexture* Out = &PingPongTarget;

        for (auto& Effect : Effects)
        {
            Out->clear(color::Transparent);
            Effect->Apply(In->getTexture(), *Out);
            Out->display();
            std::swap(In, Out);
        }

        if (In != &TempTarget)
        {
            TempTarget.clear(color::Transparent);
            TempTarget.draw(sprite(In->getTexture()));
            TempTarget.display();
        }

        ProcessedTexture = make_shared<texture>(TempTarget.getTexture());
    }
}
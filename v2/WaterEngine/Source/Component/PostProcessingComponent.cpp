// =============================================================================
// Water Engine v2.0.0
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

        if (Owner && ProcessedTexture)
        {
            Owner->SetSprite(ProcessedTexture);
        }
    }

    void PostProcessingComponent::Tick(float DeltaTime)
    {
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
    }

    void PostProcessingComponent::AddEffect(unique<IPostProcess> Effect)
    {
        Effects.push_back(std::move(Effect));
    }

    void PostProcessingComponent::ClearEffects()
    {
        Effects.clear();
    }

    void PostProcessingComponent::ApplyEffects()
    {
        renderTexture TempTarget;
        TempTarget.resize(OriginalTexture->getSize());

        TempTarget.clear();
        TempTarget.draw(sprite(*OriginalTexture));
        TempTarget.display();

        renderTexture PingPongTarget;
        PingPongTarget.resize(OriginalTexture->getSize());

        renderTexture* In = &TempTarget;
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

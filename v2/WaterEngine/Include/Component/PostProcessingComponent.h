// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"
#include "Interface/PostProcess/IPostProcess.h"

namespace we
{
    class PostProcessingComponent : public IActorComponent
    {
    public:
        explicit PostProcessingComponent(Actor* InOwner);

        void BeginPlay() override;
        void Tick(float DeltaTime) override;
        void EndPlay() override;
        Actor* GetOwner() const override;

        void SetTexture(shared<texture> Tex);
        void AddEffect(unique<IPostProcess> Effect);
        void ClearEffects();

        shared<texture> GetProcessedTexture() const { return ProcessedTexture; }

    private:
        Actor* Owner;
        shared<texture> OriginalTexture;
        shared<texture> ProcessedTexture;
        vector<unique<IPostProcess>> Effects;

        void ApplyEffects();
    };
}

// =============================================================================
// Water Engine v2.2.4
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

        // Source the pipeline from a sub-rect of the original texture (e.g. the
        // current frame of a sprite sheet). Pass an empty optional to clear and
        // process the whole texture again. Padding leaves room around the frame
        // for effects that grow the silhouette (outline thickness, blur, etc.).
        void SetSourceRect(optional<recti> FrameRect, int Padding = 4);

        void ApplyEffects();
        void ForceRefresh();  // Apply pipeline immediately without advancing effect timers.

        shared<texture> GetProcessedTexture() const { return ProcessedTexture; }
        int             GetSourcePadding()    const { return SourcePadding; }
        bool            WritesToOwnerSprite() const { return bWriteToOwnerSprite; }
        void            SetWriteToOwnerSprite(bool b) { bWriteToOwnerSprite = b; }

    private:
        Actor* Owner;
        shared<texture> OriginalTexture;
        shared<texture> ProcessedTexture;
        vector<unique<IPostProcess>> Effects;
        optional<recti> SourceRect;
        int             SourcePadding       = 4;
        bool            bWriteToOwnerSprite = true;   // legacy callers keep old behavior
    };
}

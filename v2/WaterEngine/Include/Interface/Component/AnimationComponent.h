// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Component/IAnimationComponent.h"

namespace we
{
    struct Animation
    {
        uint8 StateID;
        uint8 SpriteSheetID;
        vec2u StartFrame;
        vec2u EndFrame;
        float FrameDuration;
        float PlaybackSpeed = 1.0f;
        bool bLoop = true;
        bool bFlipX = false;
        bool bFlipY = false;
        optional<uint8> NextState;
    };

    struct SpriteSheet
    {
        shared<texture> Tex;
        vec2u FrameSize;
        uint FramesPerRow;

        SpriteSheet() = default;
        SpriteSheet(const string& Path, vec2u InFrameSize, uint InFramesPerRow = 8);
    };

    class AnimationComponent : public IAnimationComponent
    {
    public:
        explicit AnimationComponent(Actor* InOwner);

        // IActorComponent
        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;
        virtual void EndPlay() override;
        virtual Actor* GetOwner() const override;

        // IAnimationComponent
        virtual void Transition(uint8 StateID) override;
        virtual uint8 GetCurrentState() const override { return CurrentState; }
        virtual bool IsPlaying(uint8 StateID) const override;

        virtual void SetGlobalPlaybackSpeed(float Multiplier) override;
        virtual float GetGlobalPlaybackSpeed() const override { return GlobalPlaybackSpeed; }

        virtual void SetFacing(bool bInFaceLeft) override;
        virtual bool IsFacingLeft() const override { return bFaceLeft; }

        virtual void SetActiveSpriteSheet(uint8 SheetID) override;
        virtual uint8 GetActiveSpriteSheet() const override { return ActiveSheetID; }

        // AnimationComponent specific
        void SetFrameSize(vec2u Size);
        void AddAnimation(const Animation& Anim);
        void AddSpriteSheet(uint8 SheetID, const SpriteSheet& Sheet);

        vec2u GetCurrentFrame() const { return CurrentFrame; }

    private:
        Actor* Owner;

        dictionary<uint8, SpriteSheet> SpriteSheets;
        uint8 ActiveSheetID = 0;

        dictionary<uint8, Animation> Animations;
        uint8 CurrentState = 0;
        vec2u CurrentFrame;
        float ElapsedTime = 0.0f;
        float GlobalPlaybackSpeed = 1.0f;
        bool bFaceLeft = false;

        void UpdateSpriteRect();
        void SyncOriginToFrame();
        void RecalculateFramesPerRow();
        bool AdvanceFrame();

        const SpriteSheet* GetCurrentSheet() const;
    };
}

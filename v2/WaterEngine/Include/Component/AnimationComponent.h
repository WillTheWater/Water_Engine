// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Animation/IAnimationComponent.h"
#include "Interface/Actor/IActorComponent.h"

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
        void Transition(uint8 StateID) override;
        uint8 GetCurrentState() const override { return CurrentState; }
        bool IsPlaying(uint8 StateID) const override;

        void SetGlobalPlaybackSpeed(float Multiplier) override;
        float GetGlobalPlaybackSpeed() const override { return GlobalPlaybackSpeed; }

        void SetFacing(bool bInFaceLeft) override;
        bool IsFacingLeft() const override { return bFaceLeft; }

        // AnimationComponent specific
        void AddAnimation(const Animation& Anim);
        void AddSpriteSheet(uint8 SheetID, const SpriteSheet& Sheet);
        void SetActiveSpriteSheet(uint8 SheetID);

        vec2u GetCurrentFrame() const { return CurrentFrame; }

    private:
        Actor* Owner;

        dictionary<uint8, SpriteSheet> SpriteSheets;
        uint8 ActiveSheetID = 0;
        
        SpriteSheet* GetActiveSheet();
        const SpriteSheet* GetActiveSheet() const;

        dictionary<uint8, Animation> Animations;
        uint8 CurrentState = 0;
        vec2u CurrentFrame;
        float ElapsedTime = 0.0f;
        float GlobalPlaybackSpeed = 1.0f;
        bool bFaceLeft = false;

        void UpdateSpriteRect();
        void SyncOriginToFrame();
        bool AdvanceFrame();
    };
}
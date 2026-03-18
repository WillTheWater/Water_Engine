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
        uint8 AnimationState;
        uint8 SpriteSheetID;
        vec2u StartFrame;
        vec2u EndFrame;
        float FrameDuration = 0.15f;
        float PlaybackSpeed = 1.0f;
        bool bLoop = true;
        bool bFlipX = false;
        bool bFlipY = false;
        optional<uint8> NextState;

        Animation() = default;
        
        template<typename StateEnum, typename SheetEnum>
        Animation(StateEnum InState, SheetEnum InSheetID, vec2u InStart, vec2u InEnd,
                  float InDuration = 0.15f, float InSpeed = 1.0f, bool bInLoop = true,
                  bool bInFlipX = false, bool bInFlipY = false)
            : AnimationState(static_cast<uint8>(InState))
            , SpriteSheetID(static_cast<uint8>(InSheetID))
            , StartFrame(InStart)
            , EndFrame(InEnd)
            , FrameDuration(InDuration)
            , PlaybackSpeed(InSpeed)
            , bLoop(bInLoop)
            , bFlipX(bInFlipX)
            , bFlipY(bInFlipY)
        {
            static_assert(std::is_enum_v<StateEnum>, "State must be an enum type");
            static_assert(std::is_enum_v<SheetEnum>, "SheetID must be an enum type");
        }
    };

    struct SpriteSheet
    {
        shared<texture> Texture;
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
        uint8 GetCurrentState() const override { return CurrentState.value_or(0); }
        bool IsPlaying(uint8 StateID) const override;

        void SetGlobalPlaybackSpeed(float Multiplier) override;
        float GetGlobalPlaybackSpeed() const override { return GlobalPlaybackSpeed; }

        void SetFacing(bool bInFaceLeft) override;
        bool IsFacingLeft() const override { return bFaceLeft; }

        void AddAnimation(const Animation& Anim);
        
        template<typename T>
        void AddSpriteSheet(T SheetID, const SpriteSheet& Sheet)
        {
            static_assert(std::is_enum_v<T>, "SheetID must be an enum type");
            AddSpriteSheetInternal(static_cast<uint8>(SheetID), Sheet);
        }

        template<typename T>
        void TransitionTo(T State)
        {
            static_assert(std::is_enum_v<T>, "State must be an enum type");
            TransitionToInternal(static_cast<uint8>(State));
        }
        
        template<typename T>
        bool IsPlaying(T State) const
        {
            static_assert(std::is_enum_v<T>, "State must be an enum type");
            return IsPlayingInternal(static_cast<uint8>(State));
        }

        vec2u GetCurrentFrame() const { return CurrentFrame; }

    private:
        void AddSpriteSheetInternal(uint8 SheetID, const SpriteSheet& Sheet);
        void TransitionToInternal(uint8 State);
        bool IsPlayingInternal(uint8 State) const;
        
        SpriteSheet* GetActiveSheet();
        const SpriteSheet* GetActiveSheet() const;

        void UpdateSpriteRect();
        bool AdvanceFrame();

    private:
        Actor* Owner;

        dictionary<uint8, SpriteSheet> SpriteSheets;
        dictionary<uint8, Animation> Animations;
        
        optional<uint8> CurrentState;
        vec2u CurrentFrame;
        float ElapsedTime = 0.0f;
        float GlobalPlaybackSpeed = 1.0f;
        bool bFaceLeft = false;
        
        uint8 LastSheetID = 255;
    };
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interface/Component/AnimationComponent.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Framework/World/Actor/Actor.h"
#include "Utility/Log.h"

namespace we
{
    SpriteSheet::SpriteSheet(const string& Path, vec2u InFrameSize, uint InFramesPerRow)
        : Tex(LoadAsset().LoadTextureSync(Path))
        , FrameSize(InFrameSize)
        , FramesPerRow(InFramesPerRow)
    {
    }

    AnimationComponent::AnimationComponent(Actor* InOwner)
        : Owner(InOwner)
    {
    }

    void AnimationComponent::BeginPlay()
    {
        SyncOriginToFrame();
    }

    void AnimationComponent::Tick(float DeltaTime)
    {
        if (CurrentState == 0) return;
        if (!Owner) return;

        auto it = Animations.find(CurrentState);
        if (it == Animations.end()) return;

        const Animation& Anim = it->second;

        if (Anim.SpriteSheetID != ActiveSheetID)
        {
            SetActiveSpriteSheet(Anim.SpriteSheetID);
        }

        ElapsedTime += DeltaTime * GlobalPlaybackSpeed * Anim.PlaybackSpeed;

        while (ElapsedTime >= Anim.FrameDuration)
        {
            ElapsedTime -= Anim.FrameDuration;

            if (!AdvanceFrame())
            {
                if (Anim.NextState.has_value())
                {
                    Transition(Anim.NextState.value());
                    return;
                }
                else if (!Anim.bLoop)
                {
                    return;
                }
            }
        }

        UpdateSpriteRect();
    }

    void AnimationComponent::EndPlay()
    {
    }

    Actor* AnimationComponent::GetOwner() const
    {
        return Owner;
    }

    void AnimationComponent::SetFrameSize(vec2u Size)
    {
        SpriteSheets[ActiveSheetID].FrameSize = Size;
    }

    void AnimationComponent::AddSpriteSheet(uint8 SheetID, const SpriteSheet& Sheet)
    {
        SpriteSheets[SheetID] = Sheet;

        if (SpriteSheets.size() == 1)
        {
            SetActiveSpriteSheet(SheetID);
        }
    }

    void AnimationComponent::SetActiveSpriteSheet(uint8 SheetID)
    {
        if (ActiveSheetID == SheetID) return;

        auto it = SpriteSheets.find(SheetID);
        if (it == SpriteSheets.end())
        {
            LOG("SpriteSheet ID {} not found", SheetID);
            return;
        }

        ActiveSheetID = SheetID;

        if (Owner && it->second.Tex)
        {
            Owner->SetSpriteTexture(it->second.Tex);
        }

        RecalculateFramesPerRow();
    }

    void AnimationComponent::AddAnimation(const Animation& Anim)
    {
        Animations[Anim.StateID] = Anim;
    }

    void AnimationComponent::Transition(uint8 StateID)
    {
        if (CurrentState == StateID) return;
        if (StateID == 0) return;

        auto it = Animations.find(StateID);
        if (it == Animations.end())
        {
            LOG("Animation state not found: {}", StateID);
            return;
        }

        CurrentState = StateID;
        CurrentFrame = it->second.StartFrame;
        ElapsedTime = 0.0f;

        if (it->second.SpriteSheetID != ActiveSheetID)
        {
            SetActiveSpriteSheet(it->second.SpriteSheetID);
        }

        UpdateSpriteRect();
    }

    void AnimationComponent::SetGlobalPlaybackSpeed(float Multiplier)
    {
        GlobalPlaybackSpeed = std::max(0.0f, Multiplier);
    }

    void AnimationComponent::SetFacing(bool bInFaceLeft)
    {
        if (bFaceLeft == bInFaceLeft) return;
        bFaceLeft = bInFaceLeft;
        UpdateSpriteRect();
    }

    bool AnimationComponent::IsPlaying(uint8 StateID) const
    {
        return CurrentState == StateID;
    }

    void AnimationComponent::UpdateSpriteRect()
    {
        if (!Owner) return;

        auto it = Animations.find(CurrentState);
        if (it == Animations.end()) return;

        const Animation& Anim = it->second;
        const SpriteSheet* Sheet = GetCurrentSheet();
        if (!Sheet || !Sheet->Tex) return;

        Owner->SetSpriteTexture(Sheet->Tex);

        recti TexRect;
        TexRect.position.x = CurrentFrame.y * Sheet->FrameSize.x;
        TexRect.position.y = CurrentFrame.x * Sheet->FrameSize.y;
        TexRect.size = vec2i(Sheet->FrameSize);

        Owner->SetSpriteTextureRect(TexRect);

        vec2f BaseScale = Owner->GetScale();
        float ScaleX = (bFaceLeft != Anim.bFlipX) ? -std::abs(BaseScale.x) : std::abs(BaseScale.x);
        float ScaleY = Anim.bFlipY ? -BaseScale.y : BaseScale.y;
        Owner->SetScale({ ScaleX, ScaleY });
    }

    void AnimationComponent::SyncOriginToFrame()
    {
        const SpriteSheet* Sheet = GetCurrentSheet();
        if (!Owner || !Sheet) return;

        Owner->SetOrigin(vec2f(Sheet->FrameSize).componentWiseMul({ 0.5f, 0.5f }));
    }

    void AnimationComponent::RecalculateFramesPerRow()
    {
        SpriteSheet& Sheet = SpriteSheets[ActiveSheetID];
        if (Sheet.Tex && Sheet.FrameSize.x > 0)
        {
            Sheet.FramesPerRow = Sheet.Tex->getSize().x / Sheet.FrameSize.x;
        }
    }

    bool AnimationComponent::AdvanceFrame()
    {
        auto it = Animations.find(CurrentState);
        if (it == Animations.end()) return false;

        const Animation& Anim = it->second;

        if (CurrentFrame.y < Anim.EndFrame.y ||
            (CurrentFrame.y == Anim.EndFrame.y && CurrentFrame.x < Anim.EndFrame.x))
        {
            CurrentFrame.x++;
            if (CurrentFrame.x >= SpriteSheets[ActiveSheetID].FramesPerRow)
            {
                CurrentFrame.x = 0;
                CurrentFrame.y++;
            }
            return true;
        }
        else if (Anim.bLoop)
        {
            CurrentFrame = Anim.StartFrame;
            return true;
        }

        return false;
    }

    const SpriteSheet* AnimationComponent::GetCurrentSheet() const
    {
        auto it = SpriteSheets.find(ActiveSheetID);
        return (it != SpriteSheets.end()) ? &it->second : nullptr;
    }
}

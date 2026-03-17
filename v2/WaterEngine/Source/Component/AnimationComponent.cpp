// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/AnimationComponent.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Framework/World/Actor.h"
#include "Utility/Log.h"

namespace we
{
    SpriteSheet::SpriteSheet(const string& Path, vec2u InFrameSize, uint InFramesPerRow)
        : Texture(LoadAsset().LoadTexture(Path))
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
    }

    void AnimationComponent::Tick(float DeltaTime)
    {
        if (CurrentState == 0 || !Owner) return;

        auto AnimIt = Animations.find(CurrentState);
        if (AnimIt == Animations.end()) return;

        const Animation& Anim = AnimIt->second;

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
                if (!Anim.bLoop)
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

    void AnimationComponent::AddSpriteSheet(uint8 SheetID, const SpriteSheet& Sheet)
    {
        SpriteSheets[SheetID] = Sheet;

        if (SpriteSheets.size() == 1)
        {
            SetActiveSpriteSheet(SheetID);
            SyncOriginToFrame();
        }
    }

    void AnimationComponent::SetActiveSpriteSheet(uint8 SheetID)
    {
        if (ActiveSheetID == SheetID) return;
        if (!SpriteSheets.contains(SheetID)) return;

        ActiveSheetID = SheetID;

        if (Owner)
        {
            if (auto* Sheet = GetActiveSheet())
            {
                Owner->SetSprite(Sheet->Texture);
            }
        }
    }

    void AnimationComponent::AddAnimation(const Animation& Anim)
    {
        Animations[Anim.StateID] = Anim;
    }

    void AnimationComponent::Transition(uint8 StateID)
    {
        if (CurrentState == StateID || StateID == 0) return;

        auto AnimIt = Animations.find(StateID);
        if (AnimIt == Animations.end()) return;

        CurrentState = StateID;
        CurrentFrame = AnimIt->second.StartFrame;
        ElapsedTime = 0.0f;

        if (AnimIt->second.SpriteSheetID != ActiveSheetID)
        {
            SetActiveSpriteSheet(AnimIt->second.SpriteSheetID);
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

        auto AnimIt = Animations.find(CurrentState);
        if (AnimIt == Animations.end()) return;

        const Animation& Anim = AnimIt->second;
        const SpriteSheet* Sheet = GetActiveSheet();
        if (!Sheet || !Sheet->Texture) return;

        if (ActiveSheetID != LastSpriteSheetID)
        {
            Owner->SetSprite(Sheet->Texture);
            Owner->SetSpriteOrigin(vec2f(Sheet->FrameSize) * 0.5f);
            LastSpriteSheetID = ActiveSheetID;
        }

        recti TexRect;
        TexRect.position.x = CurrentFrame.y * Sheet->FrameSize.x;
        TexRect.position.y = CurrentFrame.x * Sheet->FrameSize.y;
        TexRect.size = vec2i(Sheet->FrameSize);

        Owner->SetTextureRect(TexRect);

        vec2f BaseScale = Owner->GetScale();
        float ScaleX = (bFaceLeft != Anim.bFlipX) ? -std::abs(BaseScale.x) : std::abs(BaseScale.x);
        float ScaleY = Anim.bFlipY ? -BaseScale.y : BaseScale.y;
        Owner->SetScale({ ScaleX, ScaleY });
    }

    void AnimationComponent::SyncOriginToFrame()
    {
        if (!Owner) return;
        if (const SpriteSheet* Sheet = GetActiveSheet())
        {
            Owner->SetSpriteOrigin(vec2f(Sheet->FrameSize) * 0.5f);
        }
    }

    bool AnimationComponent::AdvanceFrame()
    {
        auto AnimIt = Animations.find(CurrentState);
        if (AnimIt == Animations.end()) return false;

        const Animation& Anim = AnimIt->second;
        const SpriteSheet* Sheet = GetActiveSheet();
        if (!Sheet) return false;

        bool AtEndRow = CurrentFrame.y == Anim.EndFrame.y;
        bool AtEndCol = CurrentFrame.x == Anim.EndFrame.x;

        if (!AtEndRow || !AtEndCol)
        {
            CurrentFrame.y++;
            if (CurrentFrame.y >= Sheet->FramesPerRow)
            {
                CurrentFrame.y = 0;
                CurrentFrame.x++;
            }
            return true;
        }

        if (Anim.bLoop)
        {
            CurrentFrame = Anim.StartFrame;
            return true;
        }

        return false;
    }

    SpriteSheet* AnimationComponent::GetActiveSheet()
    {
        auto It = SpriteSheets.find(ActiveSheetID);
        return (It != SpriteSheets.end()) ? &It->second : nullptr;
    }

    const SpriteSheet* AnimationComponent::GetActiveSheet() const
    {
        auto It = SpriteSheets.find(ActiveSheetID);
        return (It != SpriteSheets.end()) ? &It->second : nullptr;
    }
}
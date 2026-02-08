// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interface/Component/AnimationComponent.h"
#include "Framework/World/Actor/Actor.h"
#include "Utility/Log.h"

namespace we
{
	AnimationComponent::AnimationComponent(Actor* InOwner, vec2u InFrameSize, uint FramesPerRow)
		: Owner(InOwner)
		, FrameSize(InFrameSize)
		, FramesPerRow{FramesPerRow}
	{
	}

	void AnimationComponent::BeginPlay()
	{
		RecalculateFramesPerRow();
		SyncOriginToFrame();
	}

	void AnimationComponent::Tick(float DeltaTime)
	{
		if (CurrentState == 0) return;
		if (!Owner || !Owner->HasSprite()) return;

		auto it = Animations.find(CurrentState);
		if (it == Animations.end()) return;

		const Animation& Anim = it->second;

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
		FrameSize = Size;
		RecalculateFramesPerRow();
		if (CurrentState != 0)
		{
			UpdateSpriteRect();
		}
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
		if (!Owner || !Owner->HasSprite()) return;

		auto it = Animations.find(CurrentState);
		if (it == Animations.end()) return;

		const Animation& Anim = it->second;

		recti TexRect;
		TexRect.position.x = CurrentFrame.y * FrameSize.x;
		TexRect.position.y = CurrentFrame.x * FrameSize.y;
		TexRect.size = vec2i(FrameSize);

		sprite& Spr = const_cast<sprite&>(Owner->GetSprite());
		Spr.setTextureRect(TexRect);

		vec2f BaseScale = Owner->GetScale();
		float ScaleX = (bFaceLeft != Anim.bFlipX) ? -std::abs(BaseScale.x) : std::abs(BaseScale.x);
		float ScaleY = Anim.bFlipY ? -BaseScale.y : BaseScale.y;
		Spr.setScale({ ScaleX, ScaleY });
	}

	void AnimationComponent::SyncOriginToFrame()
	{
		if (!Owner) return;
		Owner->SetOrigin(vec2f(FrameSize).componentWiseMul({ 0.5f, 0.5f }));
	}

	void AnimationComponent::RecalculateFramesPerRow()
	{
		if (!Owner || !Owner->HasSprite())
		{
			FramesPerRow = 8;
			return;
		}

		auto TextureSize = Owner->GetSprite().getTexture().getSize();
		if (FrameSize.x > 0)
		{
			FramesPerRow = TextureSize.x / FrameSize.x;
		}
	}

	bool AnimationComponent::AdvanceFrame()
	{
		auto it = Animations.find(CurrentState);
		if (it == Animations.end()) return false;

		const Animation& Anim = it->second;

		uint MaxY = (CurrentFrame.x == Anim.EndFrame.x) ? Anim.EndFrame.y : (FramesPerRow - 1);

		if (CurrentFrame.y >= MaxY && CurrentFrame.x >= Anim.EndFrame.x)
		{
			CurrentFrame = Anim.StartFrame;
			return false;
		}

		CurrentFrame.y++;
		if (CurrentFrame.y >= FramesPerRow)
		{
			CurrentFrame.y = 0; 
			CurrentFrame.x++; 
		}

		return true;
	}
}
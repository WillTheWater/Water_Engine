// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"
#include "Interface/Component/IActorComponent.h"

namespace we
{
	struct Animation
	{
		uint8 StateID;
		vec2u StartFrame;
		vec2u EndFrame;
		float FrameDuration;
		float PlaybackSpeed = 1.0f;
		bool bLoop = true;
		bool bFlipX = false;
		bool bFlipY = false;
		optional<uint8> NextState;
	};

	class AnimationComponent : public IActorComponent
	{
	public:
		explicit AnimationComponent(Actor* InOwner, vec2u InFrameSize = { 48, 48 }, uint FramesPerRow = 8);

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void EndPlay() override;
		virtual Actor* GetOwner() const override;

		void SetFrameSize(vec2u Size);
		vec2u GetFrameSize() const { return FrameSize; }

		// uint8 Enum State(Cuastom), vec2u Startframe, vec2u EndFrame, 
		// float Frame Duration, float Playback Speed = 1.f, bool Loop = true,
		// bool FlipX = false, bool FlipY = false, uint8 Auto Transition = nullopt
		void AddAnimation(const Animation& Anim);
		void Transition(uint8 StateID);

		void SetGlobalPlaybackSpeed(float Multiplier);
		float GetGlobalPlaybackSpeed() const { return GlobalPlaybackSpeed; }

		void SetFacing(bool bFaceLeft);
		bool IsFacingLeft() const { return bFaceLeft; }

		uint8 GetCurrentState() const { return CurrentState; }
		bool IsPlaying(uint8 StateID) const;

	private:
		Actor* Owner;
		vec2u FrameSize;
		uint FramesPerRow;
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
	};
}
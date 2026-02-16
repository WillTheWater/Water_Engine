//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#pragma once
//#include "Core/CoreMinimal.h"
//#include "Interface/Component/IActorComponent.h"
//
//namespace we
//{
//	struct Animation
//	{
//		uint8 StateID;
//		uint8 SpriteSheetID;
//		vec2u StartFrame;
//		vec2u EndFrame;
//		float FrameDuration;
//		float PlaybackSpeed = 1.0f;
//		bool bLoop = true;
//		bool bFlipX = false;
//		bool bFlipY = false;
//		optional<uint8> NextState;
//	};
//
//	struct SpriteSheet
//	{
//		shared<texture> Tex;
//		vec2u FrameSize;
//		uint FramesPerRow;
//
//		SpriteSheet() = default;
//		SpriteSheet(const string& Path, vec2u InFrameSize, uint InFramesPerRow = 8);
//	};
//
//	class AnimationComponent : public IActorComponent
//	{
//	public:
//		explicit AnimationComponent(Actor* InOwner);
//
//		virtual void BeginPlay() override;
//		virtual void Tick(float DeltaTime) override;
//		virtual void EndPlay() override;
//		virtual Actor* GetOwner() const override;
//
//		void SetFrameSize(vec2u Size);
//
//		// uint8 Enum State(Cuastom), vec2u Startframe, vec2u EndFrame, 
//		// float Frame Duration, float Playback Speed = 1.f, bool Loop = true,
//		// bool FlipX = false, bool FlipY = false, uint8 Auto Transition = nullopt
//		void AddAnimation(const Animation& Anim);
//		void Transition(uint8 StateID);
//
//		// Sprite sheet management
//		void AddSpriteSheet(uint8 SheetID, const SpriteSheet& Sheet);
//		void SetActiveSpriteSheet(uint8 SheetID);
//		uint8 GetActiveSpriteSheet() const { return ActiveSheetID; }
//
//		void SetGlobalPlaybackSpeed(float Multiplier);
//		float GetGlobalPlaybackSpeed() const { return GlobalPlaybackSpeed; }
//
//		void SetFacing(bool bFaceLeft);
//		bool IsFacingLeft() const { return bFaceLeft; }
//
//		uint8 GetCurrentState() const { return CurrentState; }
//		vec2u GetCurrentFrame() const { return CurrentFrame; }
//		bool IsPlaying(uint8 StateID) const;
//
//	private:
//		Actor* Owner;
//
//		dictionary<uint8, SpriteSheet> SpriteSheets;
//		uint8 ActiveSheetID = 0;
//
//		dictionary<uint8, Animation> Animations;
//		uint8 CurrentState = 0;
//		vec2u CurrentFrame;
//		float ElapsedTime = 0.0f;
//		float GlobalPlaybackSpeed = 1.0f;
//		bool bFaceLeft = false;
//
//		void UpdateSpriteRect();
//		void SyncOriginToFrame();
//		void RecalculateFramesPerRow();
//		bool AdvanceFrame();
//
//		const SpriteSheet* GetCurrentSheet() const;
//	};
//}
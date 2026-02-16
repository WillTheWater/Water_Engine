//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#include "Interface/Component/AnimationComponent.h"
//#include "Subsystem/ResourceSubsystem.h"
//#include "Framework/World/Actor/Actor.h"
//#include "Utility/Log.h"
//
//namespace we
//{
//	SpriteSheet::SpriteSheet(const string& Path, vec2u InFrameSize, uint InFramesPerRow)
//		: Tex(Asset().LoadTexture(Path))
//		, FrameSize(InFrameSize)
//		, FramesPerRow(InFramesPerRow)
//	{
//	}
//
//	AnimationComponent::AnimationComponent(Actor* InOwner)
//		: Owner(InOwner)
//	{
//	}
//
//	void AnimationComponent::BeginPlay()
//	{
//		SyncOriginToFrame();
//	}
//
//	void AnimationComponent::Tick(float DeltaTime)
//	{
//		if (CurrentState == 0) return;
//		if (!Owner) return;
//
//		auto it = Animations.find(CurrentState);
//		if (it == Animations.end()) return;
//
//		const Animation& Anim = it->second;
//
//		// Auto-switch sprite sheet if needed
//		if (Anim.SpriteSheetID != ActiveSheetID)
//		{
//			SetActiveSpriteSheet(Anim.SpriteSheetID);
//		}
//
//		ElapsedTime += DeltaTime * GlobalPlaybackSpeed * Anim.PlaybackSpeed;
//
//		while (ElapsedTime >= Anim.FrameDuration)
//		{
//			ElapsedTime -= Anim.FrameDuration;
//
//			if (!AdvanceFrame())
//			{
//				if (Anim.NextState.has_value())
//				{
//					Transition(Anim.NextState.value());
//					return;
//				}
//				else if (!Anim.bLoop)
//				{
//					return;
//				}
//			}
//		}
//
//		UpdateSpriteRect();
//	}
//
//	void AnimationComponent::EndPlay()
//	{
//	}
//
//	Actor* AnimationComponent::GetOwner() const
//	{
//		return Owner;
//	}
//
//	void AnimationComponent::SetFrameSize(vec2u Size)
//	{
//		SpriteSheets[ActiveSheetID].FrameSize = Size;
//	}
//
//	void AnimationComponent::AddSpriteSheet(uint8 SheetID, const SpriteSheet& Sheet)
//	{
//		SpriteSheets[SheetID] = Sheet;
//
//		// Auto-activate first sheet added
//		if (SpriteSheets.size() == 1)
//		{
//			SetActiveSpriteSheet(SheetID);
//		}
//	}
//
//	void AnimationComponent::SetActiveSpriteSheet(uint8 SheetID)
//	{
//		if (ActiveSheetID == SheetID) return;
//
//		auto it = SpriteSheets.find(SheetID);
//		if (it == SpriteSheets.end())
//		{
//			LOG("SpriteSheet ID {} not found", SheetID);
//			return;
//		}
//
//		ActiveSheetID = SheetID;
//
//		// Update actor's sprite texture
//		if (Owner && it->second.Tex)
//		{
//			// Create new sprite with this texture, preserving transform
//			vec2f Pos = Owner->GetPosition();
//			angle Rot = Owner->GetRotation();
//			vec2f Scale = Owner->GetScale();
//
//			// You'll need to add this method to Actor, or make AnimationComponent friend
//			// For now, assume we can update the sprite texture directly
//			// Owner->SetSpriteTexture(it->second.Tex);
//		}
//
//		RecalculateFramesPerRow();
//	}
//
//	void AnimationComponent::AddAnimation(const Animation& Anim)
//	{
//		Animations[Anim.StateID] = Anim;
//	}
//
//	void AnimationComponent::Transition(uint8 StateID)
//	{
//		if (CurrentState == StateID) return;
//		if (StateID == 0) return;
//
//		auto it = Animations.find(StateID);
//		if (it == Animations.end())
//		{
//			LOG("Animation state not found: {}", StateID);
//			return;
//		}
//
//		CurrentState = StateID;
//		CurrentFrame = it->second.StartFrame;
//		ElapsedTime = 0.0f;
//
//		// Ensure correct sheet is active
//		if (it->second.SpriteSheetID != ActiveSheetID)
//		{
//			SetActiveSpriteSheet(it->second.SpriteSheetID);
//		}
//
//		UpdateSpriteRect();
//	}
//
//	void AnimationComponent::SetGlobalPlaybackSpeed(float Multiplier)
//	{
//		GlobalPlaybackSpeed = std::max(0.0f, Multiplier);
//	}
//
//	void AnimationComponent::SetFacing(bool bInFaceLeft)
//	{
//		if (bFaceLeft == bInFaceLeft) return;
//		bFaceLeft = bInFaceLeft;
//		UpdateSpriteRect();
//	}
//
//	bool AnimationComponent::IsPlaying(uint8 StateID) const
//	{
//		return CurrentState == StateID;
//	}
//
//	void AnimationComponent::UpdateSpriteRect()
//	{
//		if (!Owner) return;
//
//		auto it = Animations.find(CurrentState);
//		if (it == Animations.end()) return;
//
//		const Animation& Anim = it->second;
//		const SpriteSheet* Sheet = GetCurrentSheet();
//		if (!Sheet || !Sheet->Tex) return;
//
//		// Update actor's sprite texture and rect
//		// This assumes Actor has a method to update texture, or you make AnimationComponent a friend
//		// For now, using your existing pattern:
//
//		if (!Owner->HasSprite())
//		{
//			// Create sprite if needed
//			// Owner->CreateSprite(*Sheet->Tex); // Add this to Actor
//			return;
//		}
//
//		// Update texture rect based on current frame
//		recti TexRect;
//		TexRect.position.x = CurrentFrame.y * Sheet->FrameSize.x;
//		TexRect.position.y = CurrentFrame.x * Sheet->FrameSize.y;
//		TexRect.size = vec2i(Sheet->FrameSize);
//
//		// Update the sprite - you'll need to ensure texture is set correctly
//		// This is where you need Actor cooperation
//		sprite& Spr = const_cast<sprite&>(Owner->GetSprite());
//		Spr.setTexture(*Sheet->Tex);  // Switch texture
//		Spr.setTextureRect(TexRect);
//
//		// Apply facing
//		vec2f BaseScale = Owner->GetScale();
//		float ScaleX = (bFaceLeft != Anim.bFlipX) ? -std::abs(BaseScale.x) : std::abs(BaseScale.x);
//		float ScaleY = Anim.bFlipY ? -BaseScale.y : BaseScale.y;
//		Spr.setScale({ ScaleX, ScaleY });
//	}
//
//	void AnimationComponent::SyncOriginToFrame()
//	{
//		const SpriteSheet* Sheet = GetCurrentSheet();
//		if (!Owner || !Sheet) return;
//
//		Owner->SetOrigin(vec2f(Sheet->FrameSize).componentWiseMul({ 0.5f, 0.5f }));
//	}
//
//	void AnimationComponent::RecalculateFramesPerRow()
//	{
//		const SpriteSheet* Sheet = GetCurrentSheet();
//		if (!Sheet || !Sheet->Tex) return;
//
//		auto TextureSize = Sheet->Tex->getSize();
//		if (Sheet->FrameSize.x > 0)
//		{
//			const_cast<SpriteSheet*>(Sheet)->FramesPerRow = TextureSize.x / Sheet->FrameSize.x;
//		}
//	}
//
//	bool AnimationComponent::AdvanceFrame()
//	{
//		auto it = Animations.find(CurrentState);
//		if (it == Animations.end()) return false;
//
//		const Animation& Anim = it->second;
//		const SpriteSheet* Sheet = GetCurrentSheet();
//		if (!Sheet) return false;
//
//		uint MaxY = (CurrentFrame.x == Anim.EndFrame.x) ? Anim.EndFrame.y : (Sheet->FramesPerRow - 1);
//
//		if (CurrentFrame.y >= MaxY && CurrentFrame.x >= Anim.EndFrame.x)
//		{
//			CurrentFrame = Anim.StartFrame;
//			return false;
//		}
//
//		CurrentFrame.y++;
//		if (CurrentFrame.y >= Sheet->FramesPerRow)
//		{
//			CurrentFrame.y = 0;
//			CurrentFrame.x++;
//		}
//
//		return true;
//	}
//
//	const SpriteSheet* AnimationComponent::GetCurrentSheet() const
//	{
//		auto it = SpriteSheets.find(ActiveSheetID);
//		if (it != SpriteSheets.end()) return &it->second;
//
//		// Fallback to first available
//		if (!SpriteSheets.empty()) return &SpriteSheets.begin()->second;
//		return nullptr;
//	}
//}
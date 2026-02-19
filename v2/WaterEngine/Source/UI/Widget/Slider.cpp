// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Slider.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"

namespace we
{
	// =========================================================================
	// Constructors
	// =========================================================================

	// Rectangle slider
	Slider::Slider(
		float TrackWidth,
		float TrackHeight,
		float ThumbWidth,
		float ThumbHeight,
		SliderOrientation Orient)
		: TrackSize{ TrackWidth, TrackHeight }
		, ThumbSize{ ThumbWidth, ThumbHeight }
		, Orientation(Orient)
		, bTrackUsesTexture(false)
		, bThumbUsesTexture(false)
	{
		InitializeShapes();
	}

	// Texture track + rectangle thumb
	Slider::Slider(
		const string& TrackTexturePath,
		float ThumbWidth,
		float ThumbHeight,
		SliderOrientation Orient)
		: ThumbSize{ ThumbWidth, ThumbHeight }
		, Orientation(Orient)
		, bThumbUsesTexture(false)
	{
		SetTrackTexture(TrackTexturePath);
		// If texture failed to load, set default track size
		if (!bTrackUsesTexture)
		{
			TrackSize = { 200.f, 10.f };
		}
		InitializeShapes();
	}

	// Rectangle track + texture thumb
	Slider::Slider(
		float TrackWidth,
		float TrackHeight,
		const string& ThumbTexturePath,
		SliderOrientation Orient)
		: TrackSize{ TrackWidth, TrackHeight }
		, Orientation(Orient)
		, bTrackUsesTexture(false)
	{
		SetThumbTexture(ThumbTexturePath);
		// If thumb texture failed to load, set default thumb size
		if (!bThumbUsesTexture)
		{
			ThumbSize = { 20.f, 30.f };
		}
		InitializeShapes();
	}

	// Texture track + texture thumb
	Slider::Slider(
		const string& TrackTexturePath,
		const string& ThumbTexturePath,
		SliderOrientation Orient)
		: Orientation(Orient)
	{
		SetTrackTexture(TrackTexturePath);
		SetThumbTexture(ThumbTexturePath);
		// If textures failed to load, set default sizes
		if (!bTrackUsesTexture)
		{
			TrackSize = { 200.f, 10.f };
		}
		if (!bThumbUsesTexture)
		{
			ThumbSize = { 20.f, 30.f };
		}
		InitializeShapes();
	}

	void Slider::InitializeShapes()
	{
		SetFocusable(true);

		// Track setup
		if (!bTrackUsesTexture)
		{
			TrackRect.emplace();
			TrackRect->setSize(TrackSize);
			TrackRect->setFillColor(TrackColor);
		}

		// Thumb setup
		if (!bThumbUsesTexture)
		{
			ThumbRect.emplace();
			ThumbRect->setSize(ThumbSize);
			ThumbRect->setFillColor(ThumbNormalColor);
			// Center origin for thumb
			ThumbRect->setOrigin({ ThumbSize.x * 0.5f, ThumbSize.y * 0.5f });
		}
		else
		{
			// Sprite origin at center
			if (ThumbSprite)
			{
				ThumbSprite->setOrigin({ ThumbSize.x * 0.5f, ThumbSize.y * 0.5f });
			}
		}

		// Widget size is track size
		SetSize(TrackSize);

		// Connect hover events
		OnHoverGained.Bind([this]() {
			bHovered = true;
			UpdateVisualState();
		});
		OnHoverLost.Bind([this]() {
			bHovered = false;
			UpdateVisualState();
		});

		UpdateThumbPosition();
		UpdateVisualState();
	}

	// =========================================================================
	// Value Management
	// =========================================================================

	void Slider::SetValue(float NewValue)
	{
		float OldValue = Value;
		Value = std::clamp(NewValue, 0.f, 1.f);
		if (Value != OldValue)
		{
			UpdateThumbPosition();
			OnValueChanged.Broadcast(Value);
		}
	}

	float Slider::CalculateValueFromMouse(const vec2f& MousePos) const
	{
		vec2f Pos = GetWorldPosition();

		if (Orientation == SliderOrientation::Horizontal)
		{
			// Calculate value based on mouse X position
			float TrackStart = Pos.x;
			float TrackEnd = Pos.x + TrackSize.x;
			float ClampedX = std::clamp(MousePos.x, TrackStart, TrackEnd);
			return (ClampedX - TrackStart) / TrackSize.x;
		}
		else
		{
			// Vertical: 0 at bottom, 1 at top (or reverse? let's do 0 at bottom, 1 at top)
			float TrackStart = Pos.y + TrackSize.y;  // bottom
			float TrackEnd = Pos.y;                   // top
			float ClampedY = std::clamp(MousePos.y, TrackEnd, TrackStart);
			// Invert because Y increases downward
			return (TrackStart - ClampedY) / TrackSize.y;
		}
	}

	void Slider::UpdateThumbPosition() const
	{
		vec2f Pos = GetWorldPosition();
		vec2f ThumbPos;

		if (Orientation == SliderOrientation::Horizontal)
		{
			// Thumb moves along X axis
			// Center of thumb = left edge + margin + value * available space
			float AvailableSpace = TrackSize.x - ThumbSize.x;
			float ThumbCenterX = Pos.x + ThumbSize.x * 0.5f + Value * AvailableSpace;
			float ThumbCenterY = Pos.y + TrackSize.y * 0.5f;
			ThumbPos = { ThumbCenterX, ThumbCenterY };
		}
		else
		{
			// Thumb moves along Y axis
			// Value 0 = bottom, Value 1 = top
			float AvailableSpace = TrackSize.y - ThumbSize.y;
			float ThumbCenterX = Pos.x + TrackSize.x * 0.5f;
			float ThumbCenterY = Pos.y + TrackSize.y - (ThumbSize.y * 0.5f + Value * AvailableSpace);
			ThumbPos = { ThumbCenterX, ThumbCenterY };
		}

		if (ThumbRect)
		{
			ThumbRect->setPosition(ThumbPos);
		}
		else if (ThumbSprite)
		{
			ThumbSprite->setPosition(ThumbPos);
		}
	}

	// =========================================================================
	// Rendering
	// =========================================================================

	void Slider::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		if (!IsVisible()) return;

		vec2f Pos = GetWorldPosition();
		float Depth = GetRenderDepth();

		// Track
		if (bTrackUsesTexture && TrackSprite)
		{
			TrackSprite->setPosition(Pos);
			OutDepths.push_back({ &*TrackSprite, Depth });
		}
		else if (TrackRect)
		{
			TrackRect->setPosition(Pos);
			OutDepths.push_back({ &*TrackRect, Depth });
		}

		// Thumb (at higher depth)
		UpdateThumbPosition();  // Ensure thumb is at correct position
		if (bThumbUsesTexture && ThumbSprite)
		{
			OutDepths.push_back({ &*ThumbSprite, Depth + 1.f });
		}
		else if (ThumbRect)
		{
			OutDepths.push_back({ &*ThumbRect, Depth + 1.f });
		}
	}

	// =========================================================================
	// Input Handling
	// =========================================================================

	bool Slider::Contains(const vec2f& ScreenPoint) const
	{
		vec2f Pos = GetWorldPosition();

		// Check track bounds first
		if (ScreenPoint.x >= Pos.x && ScreenPoint.x <= Pos.x + TrackSize.x &&
		    ScreenPoint.y >= Pos.y && ScreenPoint.y <= Pos.y + TrackSize.y)
		{
			return true;
		}

		// Also check thumb bounds (for when thumb extends outside track)
		vec2f ThumbPos;
		if (Orientation == SliderOrientation::Horizontal)
		{
			float AvailableSpace = TrackSize.x - ThumbSize.x;
			ThumbPos.x = Pos.x + ThumbSize.x * 0.5f + Value * AvailableSpace;
			ThumbPos.y = Pos.y + TrackSize.y * 0.5f;
		}
		else
		{
			ThumbPos.x = Pos.x + TrackSize.x * 0.5f;
			float AvailableSpace = TrackSize.y - ThumbSize.y;
			ThumbPos.y = Pos.y + TrackSize.y - (ThumbSize.y * 0.5f + Value * AvailableSpace);
		}

		vec2f HalfThumb = { ThumbSize.x * 0.5f, ThumbSize.y * 0.5f };
		return ScreenPoint.x >= ThumbPos.x - HalfThumb.x &&
		       ScreenPoint.x <= ThumbPos.x + HalfThumb.x &&
		       ScreenPoint.y >= ThumbPos.y - HalfThumb.y &&
		       ScreenPoint.y <= ThumbPos.y + HalfThumb.y;
	}

	void Slider::SetPressed(bool bInPressed)
	{
		bool WasPressed = bGrabbed;
		bGrabbed = bInPressed;

		if (!bInPressed && WasPressed)
		{
			// Released - nothing special needed
		}

		UpdateVisualState();
	}

	void Slider::OnPressed(const vec2f& MousePos)
	{
		// When pressing anywhere on the slider (track or thumb),
		// immediately snap the thumb to that position
		float NewValue = CalculateValueFromMouse(MousePos);
		SetValue(NewValue);
	}

	void Slider::OnDrag(const vec2f& MousePos)
	{
		if (!bGrabbed) return;

		float NewValue = CalculateValueFromMouse(MousePos);
		SetValue(NewValue);
	}

	// =========================================================================
	// Visual State
	// =========================================================================

	void Slider::UpdateVisualState()
	{
		// Update thumb color/texture rect based on state
		if (bThumbUsesTexture && ThumbSprite)
		{
			if (bThumbHasStateRects)
			{
				if (bGrabbed)
				{
					ThumbSprite->setTextureRect(ThumbPressedRect);
				}
				else if (bHovered)
				{
					ThumbSprite->setTextureRect(ThumbHoverRect);
				}
				else
				{
					ThumbSprite->setTextureRect(ThumbNormalRect);
				}
			}
		}
		else if (ThumbRect)
		{
			if (bGrabbed)
			{
				ThumbRect->setFillColor(ThumbPressedColor);
			}
			else if (bHovered)
			{
				ThumbRect->setFillColor(ThumbHoverColor);
			}
			else
			{
				ThumbRect->setFillColor(ThumbNormalColor);
			}
		}
	}

	// =========================================================================
	// Setters
	// =========================================================================

	void Slider::SetTrackColor(color InColor)
	{
		TrackColor = InColor;
		if (TrackRect)
		{
			TrackRect->setFillColor(TrackColor);
		}
	}

	void Slider::SetThumbColor(color InColor)
	{
		ThumbNormalColor = InColor;
		UpdateVisualState();
	}

	void Slider::SetThumbHoverColor(color InColor)
	{
		ThumbHoverColor = InColor;
		UpdateVisualState();
	}

	void Slider::SetThumbPressedColor(color InColor)
	{
		ThumbPressedColor = InColor;
		UpdateVisualState();
	}

	void Slider::SetTrackTexture(const string& TexturePath)
	{
		if (!TexturePath.empty())
		{
			TrackTexture = LoadAsset().LoadTextureSync(TexturePath);
			if (TrackTexture)
			{
				TrackSize = vec2f(TrackTexture->getSize());
				bTrackUsesTexture = true;
				TrackRect.reset();
				TrackSprite.emplace(*TrackTexture);
				SetSize(TrackSize);
			}
		}
	}

	void Slider::SetThumbTexture(const string& TexturePath)
	{
		if (!TexturePath.empty())
		{
			ThumbTexture = LoadAsset().LoadTextureSync(TexturePath);
			if (ThumbTexture)
			{
				ThumbSize = vec2f(ThumbTexture->getSize());
				bThumbUsesTexture = true;
				ThumbRect.reset();
				ThumbSprite.emplace(*ThumbTexture);
				ThumbSprite->setOrigin({ ThumbSize.x * 0.5f, ThumbSize.y * 0.5f });
			}
		}
	}

	void Slider::SetThumbNormalRect(const recti& Rect)
	{
		ThumbNormalRect = Rect;
		bThumbHasStateRects = true;
		UpdateVisualState();
	}

	void Slider::SetThumbHoverRect(const recti& Rect)
	{
		ThumbHoverRect = Rect;
		bThumbHasStateRects = true;
		UpdateVisualState();
	}

	void Slider::SetThumbPressedRect(const recti& Rect)
	{
		ThumbPressedRect = Rect;
		bThumbHasStateRects = true;
		UpdateVisualState();
	}
}

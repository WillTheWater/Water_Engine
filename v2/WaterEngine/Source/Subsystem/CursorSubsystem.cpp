// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CursorSubsystem.h"

namespace we
{
	CursorSubsytem::CursorSubsytem(const EngineConfig::CursorConfig& Config)
		: Config{Config}
		, CursorSize{Config.CursorSize}
		, CursorSpeed{Config.CursorSpeed}
		, bIsVisible{true}
		, CurrentPosition{0.f, 0.f}
		, DefaultShape{8.0f}  // Default radius 10px
	{
		// Set up default circle shape
		DefaultShape.setFillColor(color::White);
		DefaultShape.setOrigin({10.0f, 10.0f});  // Center the circle
		DefaultShape.setPosition(CurrentPosition);
	}

	void CursorSubsytem::Update(float DeltaTime)
	{
		// Future: cursor animation (pulse, trail effects, etc.)
		(void)DeltaTime;
	}

	void CursorSubsytem::SetSpeed(float Speed)
	{
		CursorSpeed = Speed; 
	}

	void CursorSubsytem::SetVisibility(bool Visible)
	{
		bIsVisible = Visible;
	}

	void CursorSubsytem::SetCursorSize(vec2f Size)
	{
		CursorSize = Size;
	}

	void CursorSubsytem::SetPosition(vec2f Position)
	{
		CurrentPosition = Position;
		DefaultShape.setPosition(Position);
		
		if (CursorSprite.has_value())
		{
			CursorSprite->setPosition(Position);
		}
	}

	vec2f CursorSubsytem::GetPosition() const
	{
		return CurrentPosition;
	}

	void CursorSubsytem::UpdateFromMouse(vec2f MousePosition)
	{
		SetPosition(MousePosition);
	}

	const drawable* CursorSubsytem::GetDrawable() const
	{
		if (Mode == CursorMode::Sprite && CursorSprite.has_value())
		{
			return &*CursorSprite;
		}
		return &DefaultShape;
	}

	void CursorSubsytem::SetShapeRadius(float Radius)
	{
		DefaultShape.setRadius(Radius);
		DefaultShape.setOrigin({Radius, Radius});
	}

	void CursorSubsytem::SetShapeColor(const color& Color)
	{
		DefaultShape.setFillColor(Color);
	}

	void CursorSubsytem::SetTexture(shared<texture> Tex)
	{
		if (!Tex) return;
		
		SpriteTexture = Tex;
		CursorSprite.emplace(*Tex);
		CursorSprite->setPosition(CurrentPosition);
		
		Mode = CursorMode::Sprite;
	}

	void CursorSubsytem::ClearTexture()
	{
		CursorSprite.reset();
		SpriteTexture.reset();
		Mode = CursorMode::Shape;
	}

	bool CursorSubsytem::HasTexture() const
	{
		return Mode == CursorMode::Sprite && CursorSprite.has_value();
	}

	void CursorSubsytem::ApplyCursorSize()
	{
		// Future: Apply size constraints or scaling
	}

	void CursorSubsytem::CenterCursor()
	{
		// Future: Center cursor in window
	}
}

// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	CursorSubsytem::CursorSubsytem(const EngineConfig::CursorConfig& Config, ResourceSubsystem& Resources)
		: Config{Config}
		, Resources{Resources}
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
		SetTexture();
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

	void CursorSubsytem::SetTexture()
	{
		auto Tex = Resources.LoadTextureSync(Config.CursorTexturePath);
		if (!Tex) return;
		
		CursorTexture = Tex;
		CursorSprite.emplace(*Tex);

		auto ScaleX = static_cast<float>(Config.CursorSize.x) / static_cast<float>(Tex->getSize().x);
		auto ScaleY = static_cast<float>(Config.CursorSize.y) / static_cast<float>(Tex->getSize().y);

		CursorSprite->setScale({ ScaleX, ScaleY });
		CursorSprite->setPosition(CurrentPosition);
		
		Mode = CursorMode::Sprite;
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

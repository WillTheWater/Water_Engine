// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	CursorSubsystem::CursorSubsystem()
		: CursorTexture{}
		, CursorSprite{}
		, CurrentPosition{}
		, CursorSize{42,42}
		, bIsVisible{true}
	{
		auto Cur = LoadAsset().LoadTexture("Assets/Textures/Default/defaultCursor.png");
		SetTexture(Cur);
	}

	void CursorSubsystem::SetTexture(shared<texture> Texture)
	{
		if (!Texture)
		{
			ERROR("Cursor texture is null");
			return;
		}
		auto ScaleX = static_cast<float>(CursorSize.x) / static_cast<float>(Texture->getSize().x);
		auto ScaleY = static_cast<float>(CursorSize.y) / static_cast<float>(Texture->getSize().y);

		CursorTexture = Texture;
		CursorSprite.emplace(*CursorTexture);
		CursorSprite->setScale({ ScaleX, ScaleY });
	}

	void CursorSubsystem::SetPosition(vec2f Position)
	{
		CurrentPosition = Position;
		if (CursorSprite.has_value())
		{
			CursorSprite->setPosition(CurrentPosition);
		}
	}

	const drawable* CursorSubsystem::GetDrawable() const
	{
		if (!bIsVisible || !CursorSprite.has_value())
			return nullptr;
		return &*CursorSprite;
	}
}
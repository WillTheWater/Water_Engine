// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Button.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/GameWindow.h"
#include "Subsystem/ResourceSubsystem.h"
#include "UI/Cursor/CursorSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	Button::Button(EngineSubsystem& Subsystem, const string& Label, const string& TexturePath)
		: Widget{ Subsystem }
		, Label{ Label }
		, HoverSoundPath{ EC.DefaultButtonHoverSound }
		, ClickSoundPath{ EC.DefaultButtonClickSound }
	{
		BgTexture = Asset().LoadTexture(TexturePath);
		if (BgTexture)
		{
			BgSprite.emplace(*BgTexture);
			SetSize(vec2f(BgTexture->getSize()));
			BgSprite->setOrigin(GetSize().componentWiseMul({0.5f, 0.5f}));
			BgSprite->setColor(NormalColor);
		}
		TextFont = Asset().LoadFont(EC.DefaultTitleFont);
		if (TextFont)
		{
			LabelText.emplace(*TextFont, Label, 24);
			LabelText->setFillColor(color::Black);
		}
	}

	void Button::Render(GameWindow& Window)
	{
		if (!IsVisible() || !BgSprite) return;
		BgSprite->setPosition(GetWorldPosition());
		BgSprite->setScale(GetWorldScale());
		Window.draw(*BgSprite);

		if (LabelText)
		{
			auto Bounds = LabelText->getLocalBounds();
			LabelText->setOrigin({ Bounds.position.x + Bounds.size.x * 0.5f, Bounds.position.y + Bounds.size.y * 0.5f });
			LabelText->setPosition(GetWorldPosition());
			Window.draw(*LabelText);
		}
	}

	void Button::OnMouseEnter()
	{
		bHovered = true;
		PlaySound(HoverSoundPath);
		UpdateVisualState();
	}

	void Button::OnMouseLeave()
	{
		bHovered = false;
		bPressed = false;
		PlaySound(UnhoverSoundPath);
		UpdateVisualState();
	}

	bool Button::OnMouseButtonPressed()
	{
		bPressed = true;
		PlaySound(PressedSoundPath);
		UpdateVisualState();
		return true;
	}

	void Button::OnMouseButtonReleased()
	{
		if (bPressed && bHovered)
		{
			PlaySound(ClickSoundPath);
			OnClicked.Broadcast();
		}
		bPressed = false;
		UpdateVisualState();
	}

	void Button::UpdateVisualState()
	{
		if (!BgSprite) return;
		color Target = bPressed && bHovered ? PressedColor : bHovered ? HoverColor : NormalColor;
		BgSprite->setColor(Target);
	}

	void Button::PlaySound(const string& Path)
	{
		if (!Path.empty()) Subsystem.Audio->PlaySFX(Path);
	}
}
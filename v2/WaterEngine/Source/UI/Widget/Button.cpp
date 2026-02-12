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
		: Widget{ Subsystem }, Label{ Label }
		, HoverSoundPath{ EC.DefaultButtonHoverSound }, ClickSoundPath{ EC.DefaultButtonClickSound }
	{
		BgTexture = Asset().LoadTexture(TexturePath);
		if (BgTexture)
		{
			vec2f TexSize = vec2f(BgTexture->getSize());
			SetSize(TexSize);
			BgSprite.emplace(*BgTexture);
			BgSprite->setColor(NormalColor);
		}

		TextFont = Asset().LoadFont(EC.DefaultTitleFont);
		if (TextFont)
		{
			LabelText.emplace(*TextFont, Label, 24);
			LabelText->setFillColor(color::Black);
		}
	}

	Button::Button(EngineSubsystem& Subsystem, const string& Label, const vec2f& InSize, color FillColor, color OutlineColor, float OutlineThickness)
		: Widget{ Subsystem }, Label{ Label }
		, HoverSoundPath{ EC.DefaultButtonHoverSound }, ClickSoundPath{ EC.DefaultButtonClickSound }
	{
		SetSize(InSize);
		NormalColor = FillColor;

		Background.emplace();
		Background->setSize(InSize);
		Background->setFillColor(FillColor);
		Background->setOutlineColor(OutlineColor);
		Background->setOutlineThickness(OutlineThickness);

		TextFont = Asset().LoadFont(EC.DefaultTitleFont);
		if (TextFont)
		{
			LabelText.emplace(*TextFont, Label, 24);
			LabelText->setFillColor(color::Black);
		}
	}

	void Button::Update(float DeltaTime)
	{

	}

	void Button::Render(GameWindow& Window)
	{
		if (!IsVisible()) return;

		// Draw background
		if (BgSprite)
		{
			BgSprite->setPosition(GetWorldPosition());
			BgSprite->setScale(GetWorldScale());
			Window.draw(*BgSprite);
		}
		else if (Background)
		{
			Background->setPosition(GetWorldPosition());
			Background->setScale(GetWorldScale());
			Window.draw(*Background);
		}

		if (LabelText)
		{
			auto Bounds = LabelText->getLocalBounds();
			LabelText->setOrigin({ Bounds.position.x + Bounds.size.x * 0.5f,
								   Bounds.position.y + Bounds.size.y * 0.5f });

			LabelText->setPosition(GetWorldPosition() + (Size.componentWiseMul(GetWorldScale()) * 0.5f));
			Window.draw(*LabelText);
		}

		Widget::Render(Window);
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
		color Target = bPressed && bHovered ? PressedColor : bHovered ? HoverColor : NormalColor;

		if (BgSprite)
		{
			BgSprite->setColor(Target);
		}

		// Support the rectangle background path
		if (Background)
		{
			Background->setFillColor(Target);
		}
	}

	void Button::PlaySound(const string& Path)
	{
		if (!Path.empty()) Subsystem.Audio->PlaySFX(Path);
	}
}
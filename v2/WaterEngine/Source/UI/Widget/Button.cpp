// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Button.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/GameWindow.h"
#include "Subsystem/ResourceSubsystem.h"
#include "UI/Cursor/CursorSubsystem.h"

namespace we
{
	Button::Button(EngineSubsystem& Subsystem, const string& Label, const string& TexturePath)
		: Widget{ Subsystem }
		, Label{ Label }
	{
		BgTexture = Asset().LoadTexture(TexturePath);
		if (BgTexture)
		{
			BgSprite.emplace(*BgTexture);
			Size = vec2f(BgTexture->getSize());
			BgSprite->setOrigin(Size.componentWiseMul({ 0.5f, 0.5f }));
		}
		TextFont = Asset().LoadFont(EC.DefaultTitleFont);
		if (TextFont)
		{
			LabelText.emplace(*TextFont, Label, 24);
			LabelText->setFillColor(color::Black);
		}
	}

	void Button::Update(float DeltaTime)
	{
		bool bWasHovered = bHovered;
		bool bWasPressed = bPressed;

		bHovered = Contains(Subsystem.Cursor->GetPosition());
		bPressed = bHovered && Subsystem.GUI->IsMousePressed();

		if (bWasPressed && !bPressed && bHovered)
		{
			OnClicked.Broadcast();
		}

		UpdateVisualState();
	}

	void Button::Render(GameWindow& Window)
	{
		if (!bVisible || !BgSprite) return;

		BgSprite->setPosition(GetWorldPosition());
		BgSprite->setRotation(GetWorldRotation());
		BgSprite->setScale(GetWorldScale());
		Window.draw(*BgSprite);

		if (LabelText)
		{
			sf::FloatRect TextBounds = LabelText->getLocalBounds();
			LabelText->setOrigin({ TextBounds.position.x + TextBounds.size.x / 2.f,
								   TextBounds.position.y + TextBounds.size.y / 2.f });
			LabelText->setPosition(GetWorldPosition());
			Window.draw(*LabelText);
		}
	}

	void Button::UpdateVisualState()
	{
		if (!BgSprite) return;

		color TargetColor = NormalColor;
		if (bPressed && bHovered) TargetColor = PressedColor;
		else if (bHovered) TargetColor = HoverColor;

		CurrentColor = TargetColor;
		BgSprite->setColor(CurrentColor);
	}
}
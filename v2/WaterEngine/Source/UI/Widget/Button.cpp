#include "UI/Widget/Button.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Framework/GameWindow.h"

namespace we
{
	Button::Button(const string& InLabel, const string& TexturePath)
		: Label{ InLabel }
	{
		BgTexture = Asset().LoadTexture(TexturePath);
		if (BgTexture)
		{
			BgSprite.emplace(*BgTexture);
			Size = vec2f(BgTexture->getSize());
			UpdateSprite();
		}
	}

	void Button::Update(float DeltaTime)
	{
		UpdateVisualState();
	}

	void Button::Render(GameWindow& Window)
	{
		if (!bVisible || !BgSprite) return;

		BgSprite->setPosition(Position);
		Window.draw(*BgSprite);

		// TODO: Render text label
	}

	bool Button::HandleClick(const vec2f& MousePos)
	{
		if (!bVisible) return false;
		OnPress();
		return true;
	}

	void Button::OnHover()
	{
		bHovered = true;
	}

	void Button::OnUnhover()
	{
		bHovered = false;
		bPressed = false;
	}

	void Button::OnPress()
	{
		bPressed = true;
	}

	void Button::OnRelease()
	{
		if (bPressed && bHovered)
		{
			OnClicked.Broadcast();
		}
		bPressed = false;
	}

	void Button::UpdateVisualState()
	{
		if (!BgSprite) return;

		color TargetColor = NormalColor;
		if (bPressed) TargetColor = PressedColor;
		else if (bHovered) TargetColor = HoverColor;

		CurrentColor = TargetColor;
		BgSprite->setColor(CurrentColor);
	}

	void Button::UpdateSprite()
	{
		if (!BgSprite) return;
		BgSprite->setOrigin(vec2f(BgTexture->getSize()).componentWiseMul({ 0.5f, 0.5f }));
	}
}
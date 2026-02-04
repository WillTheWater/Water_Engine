#include "UI/Widget/Button.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Framework/GameWindow.h"

namespace we
{
	Button::Button(const string& Label, const string& TexturePath)
		: Label{ Label }
	{
		BgTexture = Asset().LoadTexture(TexturePath);
		if (BgTexture)
		{
			BgSprite.emplace(*BgTexture);
			UpdateSprite();
		}
	}

	void Button::Update(float DeltaTime)
	{
		if (BgSprite)
		{
			BgSprite->setColor(CurrentColor);
		}
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
		if (!bVisible || !Contains(MousePos)) return false;

		OnClicked.Broadcast();
		return true;
	}

	void Button::UpdateSprite()
	{
		if (!BgSprite) return;

		BgSprite->setOrigin(vec2f(BgTexture->getSize()).componentWiseMul({0.5f,0.5f}));
		BgSprite->setPosition(Position);
	}
}
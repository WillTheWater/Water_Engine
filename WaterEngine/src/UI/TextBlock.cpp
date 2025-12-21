#include "UI/TextBlock.h"
#include "Framework/AssetManager.h"
#include "Framework/Renderer.h"

namespace we
{
	TextBlock::TextBlock(const string& Text, const string& FontPath, unsigned int FontSize)
		: DisplayFont{AssetManager::Get().LoadFont(FontPath)}
		, DisplayText{*DisplayFont.get(), Text, FontSize}
	{
	}

	void TextBlock::SetText(const string& InputText)
	{
		DisplayText.setString(InputText);
	}

	void TextBlock::SetFontSize(unsigned int NewSize)
	{
		DisplayText.setCharacterSize(NewSize);
	}

	void TextBlock::Render(Renderer& GameRenderer)
	{
		GameRenderer.Draw(DisplayText);
	}

	void TextBlock::UpdatePosition(const sf::Vector2f& NewLocation)
	{
		DisplayText.setPosition(NewLocation);
	}

	void TextBlock::UpdateRotation(const sf::Angle& NewRotation)
	{
		DisplayText.setRotation(NewRotation);
	}
}
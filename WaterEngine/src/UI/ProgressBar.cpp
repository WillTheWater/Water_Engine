#include "Framework/AssetManager.h"
#include "Framework/Renderer.h"
#include "UI/ProgressBar.h"

namespace we
{
	ProgressBar::ProgressBar(const sf::Vector2f& Dimensions, float FillPercentage)
		: ProgressBarFont{ AssetManager::Get().LoadFont("SpaceShooterRedux/Bonus/kenvector_future.ttf") }
		, ProgressBarText{ *ProgressBarFont.get(), "", 30 }
		, Background{ Dimensions }
		, FillBar{ Dimensions }
		, BackgroundColor{ sf::Color::White }
		, FillBarColor{ sf::Color::Red }
		, FillPercentage{ std::clamp(FillPercentage, 0.f, 1.f) }
	{
		Background.setFillColor(BackgroundColor);
		FillBar.setFillColor(FillBarColor);
		ProgressBarText.setOutlineColor(sf::Color::Black);
		ProgressBarText.setOutlineThickness(3);
		Background.setOrigin(Background.getSize() * 0.5f);
		FillBar.setOrigin({ 0.f, (Background.getSize().y - Padding * 2.f) * 0.5f });
		CenterText();

		UpdateFillBarPosition();
	}

	void ProgressBar::UpdateFillPercentage(float CurrentValue, float MaxValue)
	{
		if (MaxValue <= 0.f) return;

		FillPercentage = std::clamp(CurrentValue / MaxValue, 0.f, 1.f);

		if (FillPercentage < 0.25f)
		{
			FillBar.setFillColor(sf::Color::Red);
		}
		else if (FillPercentage < 0.60f)
		{
			FillBar.setFillColor(sf::Color::Yellow);
		}
		else
		{
			FillBar.setFillColor(sf::Color::Green);
		}

		ProgressBarText.setString(
			std::to_string(static_cast<int>(CurrentValue)) + "/" +
			std::to_string(static_cast<int>(MaxValue))
		);
		CenterText();

		sf::Vector2f bgSize = Background.getSize();
		float fillWidth = (bgSize.x - Padding * 2.f) * FillPercentage;
		float fillHeight = bgSize.y - Padding * 2.f;

		FillBar.setSize({ fillWidth, fillHeight });

		UpdateFillBarPosition();
	}

	void ProgressBar::SetBackgroundColor(const sf::Color& NewColor)
	{
		BackgroundColor = NewColor;
		Background.setFillColor(BackgroundColor);
	}

	void ProgressBar::SetFillBarColor(const sf::Color& NewColor)
	{
		FillBarColor = NewColor;
		FillBar.setFillColor(FillBarColor);
	}

	void ProgressBar::SetProgressBarDimensions(const sf::Vector2f& BackgroundSize)
	{
		Background.setSize(BackgroundSize);
		Background.setOrigin(BackgroundSize * 0.5f);

		UpdateFillBarPosition();
	}

	void ProgressBar::Render(Renderer& GameRenderer)
	{
		GameRenderer.Draw(Background);
		GameRenderer.Draw(FillBar);
		GameRenderer.Draw(ProgressBarText);
	}

	void ProgressBar::UpdatePosition(const sf::Vector2f& NewPosition)
	{
		Background.setPosition(NewPosition);
		ProgressBarText.setPosition(NewPosition);
		UpdateFillBarPosition();
	}

	void ProgressBar::UpdateRotation(const sf::Angle& NewRotation)
	{
		Background.setRotation(NewRotation);
		FillBar.setRotation(NewRotation);
		ProgressBarText.setRotation(NewRotation);
	}

	void ProgressBar::CenterText()
	{
		const auto bounds = ProgressBarText.getLocalBounds();
		ProgressBarText.setOrigin({ bounds.position.x + bounds.size.x * 0.5f,
			bounds.position.y + bounds.size.y * 0.5f });
	}

	void ProgressBar::UpdateFillBarPosition()
	{
		sf::Vector2f bgPos = Background.getPosition();
		sf::Vector2f bgSize = Background.getSize();

		FillBar.setPosition({bgPos.x - bgSize.x * 0.5f + Padding, bgPos.y});
	}
}

#include "UI/Button.h"
#include "Framework/AssetManager.h"

namespace we
{
	Button::Button(const string& TexturePath)
		: ButtonTexture{ AssetManager::Get().LoadTexture(TexturePath) }
		, ButtonSprite{ *ButtonTexture }
		, DefaultColor{ sf::Color {150, 150, 150, 255} }
		, HoverColor{ sf::Color::White }
		, PressedColor{ sf::Color {55, 55, 55, 255} }
		, bIsButtonPressed{ false }
	{
		ButtonSprite.setColor(DefaultColor);
	}

	bool Button::HandleEvent(const std::optional<sf::Event> Event)
	{
		bool Handled = false;

		// --- Mouse moved ---
		if (const auto* e = Event->getIf<sf::Event::MouseMoved>())
		{
			sf::Vector2f mousePos{
				static_cast<float>(e->position.x),
				static_cast<float>(e->position.y)
			};

			bool isInside = ButtonSprite.getGlobalBounds().contains(mousePos);

			// Hover ONLY if not pressed
			if (!bIsButtonPressed)
			{
				if (isInside)
					ButtonHover();
				else
					ButtonUp();
			}

			Handled = isInside;
		}

		// --- Mouse button pressed ---
		else if (const auto* e = Event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (e->button == sf::Mouse::Button::Left)
			{
				sf::Vector2f mousePos{
					static_cast<float>(e->position.x),
					static_cast<float>(e->position.y)
				};

				if (ButtonSprite.getGlobalBounds().contains(mousePos))
				{
					bIsButtonPressed = true;
					ButtonDown();
					Handled = true;
				}
			}
		}

		// --- Mouse button released ---
		else if (const auto* e = Event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (e->button == sf::Mouse::Button::Left && bIsButtonPressed)
			{
				sf::Vector2f mousePos{
					static_cast<float>(e->position.x),
					static_cast<float>(e->position.y)
				};

				if (ButtonSprite.getGlobalBounds().contains(mousePos))
				{
					OnButtonClicked.Broadcast();
					Handled = true;
				}

				bIsButtonPressed = false;
				ButtonUp();
			}
		}

		return Handled || Widget::HandleEvent(Event);
	}

	void Button::ButtonUp()
	{
		bIsButtonPressed = false;
		ButtonSprite.setColor(DefaultColor);
	}

	void Button::ButtonDown()
	{
		bIsButtonPressed = false;
		ButtonSprite.setColor(PressedColor);
	}

	void Button::ButtonHover()
	{
		ButtonSprite.setColor(HoverColor);
	}

	void Button::Render(Renderer& GameRenderer)
	{
		GameRenderer.Draw(ButtonSprite);
	}

	void Button::UpdatePosition(const sf::Vector2f& NewPosition)
	{
		ButtonSprite.setPosition(NewPosition);
	}

	void Button::UpdateRotation(const sf::Angle& NewRotation)
	{
		ButtonSprite.setRotation(NewRotation);
	}

	void Button::SetTexture(const shared<sf::Texture> NewTexture)
	{
		if (NewTexture)
		{
			ButtonTexture = NewTexture;
			ButtonSprite.setTexture(*ButtonTexture);
		}
	}

	void Button::ScaleImage(const sf::Vector2f& NewScale)
	{
		ButtonSprite.setScale(NewScale);
	}

	void Button::CenterOrigin()
	{
		ButtonSprite.setOrigin({ ButtonTexture->getSize().x / 2.f, ButtonTexture->getSize().y / 2.f });
	}

	sf::FloatRect Button::GetGlobalBounds()
	{
		return ButtonSprite.getGlobalBounds();
	}
}
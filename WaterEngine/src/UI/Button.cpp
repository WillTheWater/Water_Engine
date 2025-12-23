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
		bool handled = false;

		// -----------------------------
		// Mouse moved
		// -----------------------------
		if (const auto* event = Event->getIf<sf::Event::MouseMoved>())
		{
			sf::Vector2f mousePos{
				static_cast<float>(event->position.x),
				static_cast<float>(event->position.y)
			};

			bool isInside = ButtonSprite.getGlobalBounds().contains(mousePos);

			if (!bIsButtonPressed)
			{
				if (isInside)
					ButtonSprite.setColor(HoverColor);
				else
					ButtonSprite.setColor(DefaultColor);
			}

			handled = isInside;
		}

		// -----------------------------
		// Mouse button pressed
		// -----------------------------
		else if (const auto* event = Event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (event->button == sf::Mouse::Button::Left)
			{
				sf::Vector2f mousePos{
					static_cast<float>(event->position.x),
					static_cast<float>(event->position.y)
				};

				if (ButtonSprite.getGlobalBounds().contains(mousePos))
				{
					bIsButtonPressed = true;
					ButtonSprite.setColor(PressedColor);
					handled = true;
				}
			}
		}

		// -----------------------------
		// Mouse button released
		// -----------------------------
		else if (const auto* event = Event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (event->button == sf::Mouse::Button::Left && bIsButtonPressed)
			{
				sf::Vector2f mousePos{
					static_cast<float>(event->position.x),
					static_cast<float>(event->position.y)
				};

				bool isInside = ButtonSprite.getGlobalBounds().contains(mousePos);

				if (isInside)
				{
					OnButtonClicked.Broadcast();
					ButtonSprite.setColor(HoverColor);
				}
				else
				{
					ButtonSprite.setColor(DefaultColor);
				}

				bIsButtonPressed = false;
				handled = true;
			}
		}

		return handled || Widget::HandleEvent(Event);
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
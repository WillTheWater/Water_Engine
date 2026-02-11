// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/GameWindowEventHandler.h"
#include "Framework/GameWindow.h"
#include "Utility/Log.h"

namespace we
{
	void GameWindowEventHandler::operator()(const sf::Event::Closed&)
	{
		Window.EventWindowClose();
	}

	void GameWindowEventHandler::operator()(const sf::Event::Resized&)
	{
		Window.EventWindowResized();
	}

	void GameWindowEventHandler::operator()(const sf::Event::FocusLost&)
	{
		Window.EventWindowFocusLost();
	}

	void GameWindowEventHandler::operator()(const sf::Event::FocusGained&)
	{
		Window.EventWindowFocusGained();
	}

	void GameWindowEventHandler::operator()(const sf::Event::JoystickConnected&)
	{
		Window.EventJoypadConnected();
	}

	void GameWindowEventHandler::operator()(const sf::Event::JoystickDisconnected&)
	{
		Window.EventJoypadDisconnected();
	}

	void GameWindowEventHandler::operator()(const sf::Event::JoystickButtonPressed&)
	{
		Window.EventJoypadButtonPressed();
	}

	void GameWindowEventHandler::operator()(const sf::Event::JoystickButtonReleased&)
	{
		Window.EventJoypadButtonReleased();
	}

	void GameWindowEventHandler::operator()(const sf::Event::KeyPressed& Key)
	{
		if (Key.scancode == sf::Keyboard::Scan::F11)
		{
			Window.EventToggleBorderlessFullscreen();
		}
		Window.EventKeyPressed();
	}

	void GameWindowEventHandler::operator()(const sf::Event::KeyReleased&)
	{
		Window.EventKeyReleased();
	}

	void GameWindowEventHandler::operator()(const sf::Event::MouseMoved&)
	{

	}

	void GameWindowEventHandler::operator()(const sf::Event::MouseButtonPressed&)
	{
		Window.EventMouseButtonPressed();
	}

	void GameWindowEventHandler::operator()(const sf::Event::MouseButtonReleased&)
	{
		Window.EventMouseButtonReleased();
	}
}
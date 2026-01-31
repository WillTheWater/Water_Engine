// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

#include <sfml/Graphics.hpp>

#include "Framework/GameWindowEventHandler.h"

namespace we
{
	class GameWindow : public sf::RenderWindow
	{
	public:
		GameWindow();

	public:
		void HandleEvents();
		std::function<void(vec2u)> OnResize;

	private:
		vec2i WindowedPosition;
		bool bIsFullscreen;

	private:
		friend GameWindowEventHandler;

		void ApplyWindowSettings();
		void CreateGameWindow(const sf::VideoMode& Mode, uint Style, sf::State State = sf::State::Windowed);
		void SetWindowIcon();

		void EventWindowClose();
		void EventWindowResized();
		void EventToggleBorderlessFullscreen();
		void EventWindowFocusLost();
		void EventWindowFocusGained();
		
		void EventJoypadConnected();
		void EventJoypadDisconnected();
		void EventJoypadButtonPressed();
		void EventJoypadButtonReleased();
		
		void EventKeyPressed();
		void EventKeyReleased();
		
		void EventMouseButtonPressed();
		void EventMouseButtonReleased();

	};
}
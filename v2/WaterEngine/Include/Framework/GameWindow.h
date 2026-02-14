// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include <sfml/Graphics.hpp>

#include "Utility/CoreMinimal.h"
#include "Framework/GameWindowEventHandler.h"
#include "Subsystem/GUISubsystem.h"
#include "Utility/Delegate.h"

namespace we
{
	class GameWindow : public sf::RenderWindow
	{
	public:
		GameWindow();

	public:
		Delegate<vec2u> OnResize;

	protected:
		void onResize() override;

	private:
		vec2i WindowedPosition;
		bool bIsFullscreen;

	private:
		friend GameWindowEventHandler;
		friend GUISubsystem;

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
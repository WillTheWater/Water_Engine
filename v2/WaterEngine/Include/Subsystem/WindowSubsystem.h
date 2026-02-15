// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <sfml/Graphics.hpp>
#include "Framework/EventHandling/GameWindowEventHandler.h"


namespace we
{
	class WindowSubsystem : public sf::RenderWindow
	{
	public:
		WindowSubsystem();

		view getConstrainedView() const;

	protected:
		void onResize() override;

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
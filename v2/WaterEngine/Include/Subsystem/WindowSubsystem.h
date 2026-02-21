// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <SFML/Graphics.hpp>
#include "Framework/EventHandling/GameWindowEventHandler.h"

namespace we
{
	struct WindowConfig
	{
		stringView WindowName;
		vec2f RenderResolution;
		vec2f AspectRatio;
		vec2f WindowMinimumSize;
		stringView WindowIcon;
		bool FullscreenMode;
		bool VsyncEnabled;
		float TargetFPS;
		bool EnableKeyRepeat;
		bool DisableSFMLLogs;
	};

	class WindowSubsystem : public sf::RenderWindow
	{
	public:
		explicit WindowSubsystem(const WindowConfig& Config);

		void HandleEvent(const sf::Event& Event);
		vec2f GetMousePosition() const;

		// Fullscreen
		void SetFullscreen(bool bFullscreen);
		bool IsFullscreen() const { return bIsFullscreen; }

		// VSync
		void SetVSync(bool bEnabled);
		bool IsVSync() const { return Config.VsyncEnabled; }

	protected:
		void onResize() override;

	private:
		bool bIsFullscreen;
		bool bIsResizing = false;
		WindowConfig Config; 

	private:
		friend GameWindowEventHandler;

		void Init();
		void ApplyWindowSettings();
		void CreateGameWindow(const sf::VideoMode& Mode, uint Style, sf::State State = sf::State::Windowed);
		void SetWindowIcon();
		void EventWindowClose();
		void EventToggleBorderlessFullscreen();
	};
}
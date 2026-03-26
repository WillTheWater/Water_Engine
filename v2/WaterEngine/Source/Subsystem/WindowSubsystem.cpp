// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WindowSubsystem.h"
#include "Core/EngineConfig.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	WindowSubsystem::WindowSubsystem()
	{
		WindowedSize = WEConfig.Window.DefaultWindowSize;
		create(window(WindowedSize), WEConfig.Window.WindowTitle, sf::Style::Default);
		setVerticalSyncEnabled(WEConfig.Window.bVSyncEnabled);
		setMouseCursorVisible(WEConfig.Window.bMouseCursorVisible);
		setKeyRepeatEnabled(WEConfig.Window.bKeyRepeatEnabled);
		SetIcon(WEConfig.Window.WindowIcon);
	}

	void WindowSubsystem::SetIcon(const string& Path)
	{
		if (Path.empty()) return;
		auto IconTexture = LoadAsset().LoadTexture(Path);
		if (IconTexture)
		{
			image Icon = IconTexture->copyToImage();
			setIcon(Icon.getSize(), Icon.getPixelsPtr());
		}
	}

	void WindowSubsystem::HandleEvent(const event& Event)
	{
		WindowEventHandler Handler{ *this };
		Event.visit(Handler);
	}

	void WindowSubsystem::ToggleFullscreen()
	{
		SetFullscreen(!bFullscreen);
	}

	void WindowSubsystem::SetFullscreen(bool bEnabled)
	{
		if (bFullscreen == bEnabled)
			return;

		// Save windowed size before going fullscreen
		if (!bFullscreen && bEnabled)
		{
			WindowedSize = getSize();
		}

		bFullscreen = bEnabled;
		close();

		if (bFullscreen)
		{
			// Borderless fullscreen at desktop resolution
			auto Desktop = sf::VideoMode::getDesktopMode();
			create(window({Desktop.size.x, Desktop.size.y}),
				WEConfig.Window.WindowTitle,
				sf::Style::None);
		}
		else
		{
			// Windowed mode
			create(window(WindowedSize),
				WEConfig.Window.WindowTitle,
				sf::Style::Default);
		}

		// Re-apply settings
		setVerticalSyncEnabled(WEConfig.Window.bVSyncEnabled);
		setMouseCursorVisible(WEConfig.Window.bMouseCursorVisible);
		setKeyRepeatEnabled(WEConfig.Window.bKeyRepeatEnabled);
		SetIcon(WEConfig.Window.WindowIcon);

		OnResize.Broadcast(getSize());
	}

	void WindowSubsystem::EventResize()
	{
		OnResize.Broadcast(getSize());
	}

	void WindowSubsystem::EventMouseMoved(vec2f NewPosition)
	{
		OnMouseMove.Broadcast(NewPosition);
	}
}

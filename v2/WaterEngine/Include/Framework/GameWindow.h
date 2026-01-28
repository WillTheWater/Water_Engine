// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

#include <sfml/Graphics.hpp>


namespace we
{
	class GameWindow : public sf::RenderWindow
	{
	public:
		GameWindow();

		// Switch between windowed and borderless fullscreen
		void ToggleBorderlessFullscreen();

	private:
		// Keep track of previous windowed size & position
		vec2i WindowedPosition;
		bool bIsFullscreen = false;
	};
}
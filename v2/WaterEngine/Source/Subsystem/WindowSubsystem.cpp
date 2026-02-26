// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WindowSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	WindowSubsystem::WindowSubsystem()
	{
		const sf::VideoMode Mode({ 1280, 720 });
		const sf::State State = sf::State::Windowed;
		const uint Style = sf::Style::Default;

		create(Mode, "Water Engine", Style, State);
		setFramerateLimit(60);

		LOG("Window created: {}x{}", Mode.size.x, Mode.size.y);
	}
}

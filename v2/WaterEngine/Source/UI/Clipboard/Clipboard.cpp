// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <SFML/Window/Clipboard.hpp>

#include "UI/Clipboard/Clipboard.h"

namespace we
{
	void Clipboard::SetString(const sf::String& Text)
	{
		sf::Clipboard::setString(Text);
	}

	sf::String Clipboard::GetString() const
	{
		return sf::Clipboard::getString();
	}
}
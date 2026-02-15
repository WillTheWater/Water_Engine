// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h
#include <SFML/System/String.hpp>

namespace we
{
	class Clipboard
	{
	public:
		void SetString(const sf::String& Text);
		sf::String GetString() const;
	};
}
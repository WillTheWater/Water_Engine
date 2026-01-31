// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <SFML/Audio/Listener.hpp>

#include "Subsystem/AudioSubsystem.h"

namespace we
{
	void AudioSubsystem::SetGlobalVolume(float Volume)
	{
		sf::Listener::setGlobalVolume(Volume);
	}

	float AudioSubsystem::GetGlobalVolume() const
	{
		return sf::Listener::getGlobalVolume();
	}

	void AudioSubsystem::SetListenerPosition(vec3f Position)
	{
		sf::Listener::setPosition(Position);
	}

	vec3f AudioSubsystem::GetListenerPosition() const
	{
		return sf::Listener::getPosition();
	}

	void AudioSubsystem::SetListenerDirection(vec3f Direction)
	{
		sf::Listener::setDirection(Direction);
	}

	vec3f AudioSubsystem::GetListenerDirection() const
	{
		return sf::Listener::getDirection();
	}
}
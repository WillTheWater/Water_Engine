// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
	class AudioSubsystem
	{
	public:
		void SetGlobalVolume(float Volume);
		float GetGlobalVolume() const;

		void SetListenerPosition(vec3f Position);
		vec3f GetListenerPosition() const;

		void SetListenerDirection(vec3f Direction);
		vec3f GetListenerDirection() const;
	};
}
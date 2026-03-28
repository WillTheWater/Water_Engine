// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

class b2Body;

namespace we
{
	class IPhysicsContactListener
	{
	public:
		virtual ~IPhysicsContactListener() = default;
		virtual void OnComponentBeginOverlap(b2Body* OtherBody) = 0;
		virtual void OnComponentEndOverlap(b2Body* OtherBody) = 0;
	};
}
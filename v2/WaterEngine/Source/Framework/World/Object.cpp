// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Object.h"

namespace we
{
	Object::Object()
		: bIsPendingDestroy{ false }
	{
	}

	Object::~Object() = default;

	void Object::Destroy()
	{
		OnDestroy.Broadcast(this);
		bIsPendingDestroy = true;
	}

	bool Object::IsPendingDestroy() const
	{
		return bIsPendingDestroy;
	}
}

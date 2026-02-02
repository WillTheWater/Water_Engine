// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Object/Object.h"
#include "Utility/Log.h"

namespace we
{
	Object::Object()
		: bIsPendingDestroy{false}
	{
	}

	Object::~Object()
	{
		LOG("Object Destroyed")
	}

	void Object::Destroy()
	{
		//OnDestroy.Broadcast(this);
		bIsPendingDestroy = true;
	}

	bool Object::IsPendingDestroy() const
	{
		return bIsPendingDestroy;
	}

	weak<Object> Object::GetObject()
	{
		return weak_from_this();
	}

	weak<const Object> Object::GetObject() const
	{
		return weak_from_this();
	}
}
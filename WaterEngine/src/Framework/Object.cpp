#include "Framework/Object.h"
#include "Framework/Core.h"

namespace we
{
	Object::Object()
		: bIsPendingDestroy{ false }
	{

	}

	Object::~Object()
	{
	}

	void Object::Destroy()
	{
		OnDestroy.Broadcast(this);
		bIsPendingDestroy = true;
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
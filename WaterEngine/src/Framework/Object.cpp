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
		LOG("Object Destroyed");
	}

	void Object::Destroy()
	{
		bIsPendingDestroy = true;
	}

}
#pragma once
#include <box2d/box2d.h>
#include "Framework/Core.h"

namespace we
{
	class Actor;

	class PhysicsSystem
	{
	public:
		static PhysicsSystem& Get();

	protected:
		PhysicsSystem();

	private:
		static unique<PhysicsSystem> Instance;
	};
}
#pragma once
#include "Framework/World.h"
#include "Framework/Core.h"

namespace we
{
	class PlayerSpaceship;

	class LevelOne : public World
	{
	public:
		LevelOne(Application* OwningApp);


	private:
		weak<PlayerSpaceship> TestPlayer;
	};
}
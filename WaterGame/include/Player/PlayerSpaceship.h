#pragma once
#include "Spaceship/Spaceship.h"

namespace we
{
	class PlayerSpaceship : public Spaceship
	{
	public:
		PlayerSpaceship(World* OwningWorld, const string& TexturePath = "SpaceShooterRedux/PNG/playerShip1_orange.png");
	};
}
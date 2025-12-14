#include "Player/PlayerSpaceship.h"

namespace we
{
	PlayerSpaceship::PlayerSpaceship(World* OwningWorld, const string& TexturePath)
		: Spaceship{OwningWorld, TexturePath}
	{
	}
}
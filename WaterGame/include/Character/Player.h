#pragma once

#include "Character/Character.h"

namespace we
{
	class PlayerCharacter : public Character
	{
	public:
		PlayerCharacter(World* OwningWorld, const std::string& TexturePath = "");
	};
}
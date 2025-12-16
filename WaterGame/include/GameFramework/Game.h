#pragma once
#include <Framework/Application.h>
#include <Framework/Core.h>

namespace we
{
	class Actor;
	class PlayerSpaceship;
	class Spaceship;

	class Game : public Application
	{
	public:
		Game();

		virtual void Tick(float DeltaTime) override;

	private:
		weak<PlayerSpaceship> TestPlayer;
		weak<Spaceship> Tester;
	};
}
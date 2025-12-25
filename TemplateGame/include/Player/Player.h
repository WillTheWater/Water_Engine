#pragma once
#include "Framework/Object.h"
#include "Framework/Delegate.h"

namespace we
{
	class World;
	class PlayerSpaceship;

	class Player : public Object
	{
	public:
		Player();

		weak<PlayerSpaceship> SpawnSpaceship(World* World);
		const weak<PlayerSpaceship> GetPlayerSpaceship() const { return PlayersSpaceship; }

		void AddLives(unsigned int Lives);
		int GetLives() const { return PlayerLives; }
		void AddScore(unsigned int Amount);
		int GetScore() const { return PlayerScore; }

		Delegate<int> OnLivesChanged;
		Delegate<int> OnScoreChanged;
		Delegate<> OnPlayerDeath;


	private:
		unsigned int PlayerLives;
		unsigned int PlayerScore;
		weak<PlayerSpaceship> PlayersSpaceship;
	};
}
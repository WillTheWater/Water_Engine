#include "Player/Player.h"
#include "Framework/World.h"
#include "Player/PlayerSpaceship.h"


namespace we
{
	Player::Player()
		: PlayerLives{3}
		, PlayerScore{0}
		, PlayersSpaceship{}
	{
	}

	weak<PlayerSpaceship> Player::SpawnSpaceship(World* World)
	{
		if (PlayerLives > 0)
		{
			PlayerLives--;
			OnLivesChanged.Broadcast(PlayerLives);
			PlayersSpaceship = World->SpawnActor<PlayerSpaceship>();
			PlayersSpaceship.lock()->SetActorLocation(sf::Vector2f{ static_cast<float>(World->GetWindowSize().x / 2), static_cast<float>(World->GetWindowSize().y - 100.f) });
		}
		else
		{
			OnPlayerDeath.Broadcast();
		}
	}

	void Player::AddLives(unsigned int Lives)
	{
		if (Lives > 0)
		{
			PlayerLives += Lives;
			OnLivesChanged.Broadcast(PlayerLives);
		}
	}

	void Player::AddScore(unsigned int Amount)
	{
		if (Amount > 0)
		{
			PlayerScore += Amount;
			OnScoreChanged.Broadcast(PlayerScore);
		}
	}
}
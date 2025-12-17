#include "Levels/GameLevelOne.h"
#include "Framework/Actor.h"
#include "Framework/Assetmanager.h"
#include "Player/PlayerSpaceship.h"
#include "Enemy/Fighter.h"

namespace we
{
	LevelOne::LevelOne(Application* OwningApp)
		: World{OwningApp}
	{
		TestPlayer = SpawnActor<PlayerSpaceship>();
		TestPlayer.lock()->SetActorLocation(sf::Vector2f{ static_cast<float>(GetWindowSize().x / 2), static_cast<float>(GetWindowSize().y / 2) });

		weak<Fighter> Tester = SpawnActor<Fighter>();
		Tester.lock()->SetActorLocation(sf::Vector2f{ static_cast<float>(GetWindowSize().x / 2), 10 });
	}
}
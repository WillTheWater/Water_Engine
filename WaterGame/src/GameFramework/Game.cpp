#include "GameFramework/Game.h"
#include "Framework/World.h"
#include "Framework/Actor.h"
#include "Framework/Assetmanager.h"
#include "Player/PlayerSpaceship.h"
#include "config.h"

we::Application* GetApplication()
{
	return new we::Game{};
}

namespace we
{
	Game::Game()
		: Application{1920, 1080, "Game Window", sf::Style::Default}
	{
		AssetManager::Get().SetAssetRootDirctory(GetAssetDirectory());
		weak<World> NewWorld = LoadWorld<World>();
		TestPlayer = NewWorld.lock()->SpawnActor<PlayerSpaceship>();
		TestPlayer.lock()->SetActorLocation(sf::Vector2f{ static_cast<float>(GetWindowSize().x / 2), static_cast<float>(GetWindowSize().y / 2) });

		Tester = NewWorld.lock()->SpawnActor<Spaceship>();
		Tester.lock()->SetTexture("SpaceShooterRedux/PNG/playerShip1_orange.png");
		Tester.lock()->SetActorLocation(sf::Vector2f{ 250,250 });
		Tester.lock()->SetActorID(EActorID::Enemy);
		Tester.lock()->SetPhysicsEnabled(true);
	}
	void Game::Tick(float DeltaTime)
	{
	}
}
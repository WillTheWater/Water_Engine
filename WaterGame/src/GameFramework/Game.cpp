#include "GameFramework/Game.h"
#include "Framework/World.h"
#include "Framework/Actor.h"
#include "Framework/Assetmanager.h"
#include "Character/Character.h"
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
		AssetManager::GetAssetManager().SetAssetRootDirctory(GetAssetDirectory());
		weak<World> NewWorld = LoadWorld<World>();
		NewWorld.lock()->SpawnActor<Actor>();
		TestPlayer = NewWorld.lock()->SpawnActor<Character>();
		TestPlayer.lock()->SetTexture(GetAssetDirectory() + "character/idle/idle_down_40x40.png");
		TestPlayer.lock()->SetActorLocation(sf::Vector2f{1920/2,1080/2});
		TestPlayer.lock()->SetActorScale(2.f);
		TestPlayer.lock()->SetVelocity(sf::Vector2f{ 0.f, -200.f });
	}
	void Game::Tick(float DeltaTime)
	{
	}
}
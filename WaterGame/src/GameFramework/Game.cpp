#include "GameFramework/Game.h"
#include "Framework/World.h"
#include "Framework/Actor.h"
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
		weak<World> NewWorld = LoadWorld<World>();
		NewWorld.lock()->SpawnActor<Actor>();
		ActorToDestroy = NewWorld.lock()->SpawnActor<Actor>();
		ActorToDestroy.lock()->SetTexture(GetAssetDirectory() + "character/idle/idle_down_40x40.png");
		ActorToDestroy.lock()->SetActorLocation(sf::Vector2f{1920/2,1080/2});
		ActorToDestroy.lock()->SetActorScale(2.f);
	}
	void Game::Tick(float DeltaTime)
	{
	}
}
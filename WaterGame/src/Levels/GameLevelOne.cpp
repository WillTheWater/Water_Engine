#include "Levels/GameLevelOne.h"
#include "Framework/Actor.h"
#include "Framework/Assetmanager.h"
#include "Player/PlayerSpaceship.h"
#include "Enemy/Fighter.h"
#include "Framework/TimerManager.h"
#include "GameMode/Level.h"
#include "Levels/FighterLevel.h"

namespace we
{
	LevelOne::LevelOne(Application* OwningApp)
		: World{OwningApp}
	{
		TestPlayer = SpawnActor<PlayerSpaceship>();
		TestPlayer.lock()->SetActorLocation(sf::Vector2f{ static_cast<float>(GetWindowSize().x / 2), static_cast<float>(GetWindowSize().y / 2) });

	}

	void LevelOne::BeginPlay()
	{
	}

	void LevelOne::Tick(float DeltaTime)
	{
	}

	void LevelOne::InitLevels()
	{
		AddLevel(shared<FighterLevel>{new FighterLevel{ this }});
	}
}
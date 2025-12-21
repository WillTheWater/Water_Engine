#include "Levels/GameLevelOne.h"
#include "Framework/Actor.h"
#include "Framework/Assetmanager.h"
#include "Player/PlayerSpaceship.h"
#include "Enemy/Fighter.h"
#include "Framework/TimerManager.h"
#include "GameMode/Level.h"
#include "Levels/FighterLevel.h"
#include "Levels/TwinFighterLevel.h"
#include "Levels/DestroyerLevel.h"
#include "GameMode/LevelTransition.h"
#include "GameFramework/PlayerManager.h"
#include "Widgets/GameHUD.h"

namespace we
{
	LevelOne::LevelOne(Application* OwningApp)
		: World{OwningApp}
	{
		
	}

	void LevelOne::BeginPlay()
	{
		Player& NewPlayer = PlayerManager::Get().CreatePlayer();
		APlayer = NewPlayer.SpawnSpaceship(this);
		APlayer.lock()->OnActorDestroyed.Bind(GetObject(), &LevelOne::PlayerDied);
		GameplayHud = CreateHUD<GameplayHUD>();
	}

	void LevelOne::Tick(float DeltaTime)
	{
	}

	void LevelOne::InitLevels()
	{
		AddLevel(shared<DestroyerLevel>{new DestroyerLevel{ this }});
		AddLevel(shared<DestroyerLevel>{new DestroyerLevel{ this }});
		AddLevel(shared<LevelTransition>{new LevelTransition{ this, 3.f }});
		AddLevel(shared<TwinFighterLevel>{new TwinFighterLevel{ this }});
		AddLevel(shared<LevelTransition>{new LevelTransition{ this, 3.f }});
		AddLevel(shared<FighterLevel>{new FighterLevel{ this }});
	}

	void LevelOne::PlayerDied(Actor* Player)
	{
		APlayer = PlayerManager::Get().GetPlayer()->SpawnSpaceship(this);
		if (!APlayer.expired())
		{
			APlayer.lock()->OnActorDestroyed.Bind(GetObject(), &LevelOne::PlayerDied);
		}
		else
		{
			GameOver();
		}
	}
	void LevelOne::GameOver()
	{
		LOG("Game Over!")
	}
}
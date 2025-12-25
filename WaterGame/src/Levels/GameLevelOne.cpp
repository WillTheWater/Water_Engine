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
#include "Levels/FinalLevel.h"
#include "Levels/BossLevel.h"
#include "GameMode/LevelTransition.h"
#include "GameFramework/PlayerManager.h"
#include "Widgets/GameHUD.h"
#include "Framework/Application.h"
#include "Levels/MainMenu.h"

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
		GameplayHud.lock()->OnQuitButtonClicked.Bind(GetObject(), &LevelOne::QuitGame);
		GameplayHud.lock()->OnRestartButtonClicked.Bind(GetObject(), &LevelOne::RestartGame);
	}

	void LevelOne::Tick(float DeltaTime)
	{
	}

	void LevelOne::EndLevels()
	{
		GameplayHud.lock()->GameComplete(true);
	}

	void LevelOne::InitLevels()
	{
		AddLevel(shared<BossLevel>{new BossLevel{ this }});
		AddLevel(shared<TwinFighterLevel>{new TwinFighterLevel{ this }});
		AddLevel(shared<LevelTransition>{new LevelTransition{ this, 10.f }});
		AddLevel(shared<DestroyerLevel>{new DestroyerLevel{ this }});
		AddLevel(shared<FinalLevel>{new FinalLevel{ this }});
		AddLevel(shared<FighterLevel>{new FighterLevel{ this }});
		AddLevel(shared<FinalLevel>{new FinalLevel{ this }});
		AddLevel(shared<LevelTransition>{new LevelTransition{ this, 15.f }});
		AddLevel(shared<BossLevel>{new BossLevel{ this }});
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
		GameplayHud.lock()->GameComplete(false);
	}

	void LevelOne::QuitGame()
	{
		GetApplication()->QuitApplication();
	}

	void LevelOne::RestartGame()
	{
		PlayerManager::Get().Reset();
		GetApplication()->LoadWorld<LevelOne>();
	}
}
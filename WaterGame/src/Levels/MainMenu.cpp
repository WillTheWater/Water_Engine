#include "Levels/MainMenu.h"
#include "Widgets/MainMenuHUD.h"
#include "Framework/Application.h"
#include "Levels/GameLevelOne.h"

namespace we
{
	MainMenu::MainMenu(Application* OwningApp)
		: World{OwningApp}
	{
		MenuHUD = CreateHUD<MainMenuHUD>();
	}

	void MainMenu::BeginPlay()
	{
		MenuHUD.lock()->OnStartButtonClicked.Bind(GetObject(), &MainMenu::StartGame);
		MenuHUD.lock()->OnQuitButtonClicked.Bind(GetObject(), &MainMenu::QuitGame);
	}

	void MainMenu::Tick(float DeltaTime)
	{
	}

	void MainMenu::StartGame()
	{
		GetApplication()->LoadWorld<LevelOne>();
	}

	void MainMenu::QuitGame()
	{
		GetApplication()->QuitApplication();
	}
}
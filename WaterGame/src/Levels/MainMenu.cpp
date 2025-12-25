#include "Levels/MainMenu.h"
#include "Widgets/MainMenuHUD.h"
#include "Framework/Application.h"
#include "Framework/PropActor.h"
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
		SetBackground();
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

	void MainMenu::SetBackground()
	{
		auto BG = SpawnActor<Prop>("SpaceShooterRedux/Backgrounds/background.png");
		auto Stars = SpawnActor<Prop>("SpaceShooterRedux/Backgrounds/stars.png");
		BG.lock()->CenterPivot();
		BG.lock()->SetActorLocation({ GetWindowSize().x / 2.f, GetWindowSize().y / 2.f });
		Stars.lock()->CenterPivot();
		Stars.lock()->SetTextureTiled(true);
		Stars.lock()->SetIsMoving(true);
		Stars.lock()->SetVelocity(sf::Vector2f{ 0.f, 30.f });
		Stars.lock()->SetActorLocation({ GetWindowSize().x / 2.f, GetWindowSize().y / 2.f });
	}
}
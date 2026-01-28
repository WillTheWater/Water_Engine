#include "Widgets/MainMenuHUD.h"

namespace we
{
	MainMenuHUD::MainMenuHUD()
		: StartButton{ "SpaceShooterRedux/PNG/UI/buttonBlue.png" }
		, StartButtonText{"Start"}
		, QuitButton{ "SpaceShooterRedux/PNG/UI/buttonBlue.png" }
		, QuitButtonText{"Quit"}
	{

	}

	void MainMenuHUD::Render(Renderer& GameRenderer)
	{
		StartButton.NativeRender(GameRenderer);
		StartButtonText.NativeRender(GameRenderer);
		QuitButton.NativeRender(GameRenderer);
		QuitButtonText.NativeRender(GameRenderer);
	}

	void MainMenuHUD::Tick(float DeltaTime)
	{
	}

	bool MainMenuHUD::HandleEvent(const optional<sf::Event> Event)
	{
		return StartButton.HandleEvent(Event) || QuitButton.HandleEvent(Event) || HUD::HandleEvent(Event);
	}

	void MainMenuHUD::Initialize(Renderer& GameRenderer)
	{
		InitializeButtons(GameRenderer.GetViewportSize());
		StartButton.OnButtonClicked.Bind(GetObject(), &MainMenuHUD::StartButtonClicked);
		QuitButton.OnButtonClicked.Bind(GetObject(), &MainMenuHUD::QuitButtonClicked);
	}

	void MainMenuHUD::StartButtonClicked()
	{
		OnStartButtonClicked.Broadcast();
	}

	void MainMenuHUD::QuitButtonClicked()
	{
		OnQuitButtonClicked.Broadcast();
	}

	void MainMenuHUD::InitializeButtons(const sf::Vector2u& ViewportSize)
	{
		StartButton.CenterOrigin();
		StartButtonText.CenterOrigin();
		StartButtonText.SetColor(sf::Color::Black);
		QuitButton.CenterOrigin();
		QuitButtonText.CenterOrigin();
		QuitButtonText.SetColor(sf::Color::Black);
		StartButton.SetWidgetPosition({ ViewportSize.x / 2.f, ViewportSize.y / 2.f });
		StartButtonText.SetWidgetPosition(StartButton.GetWidgetPosition());
		QuitButton.SetWidgetPosition({ ViewportSize.x / 2.f, ViewportSize.y / 2.f + 100});
		QuitButtonText.SetWidgetPosition(QuitButton.GetWidgetPosition());
	}
}
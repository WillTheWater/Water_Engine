#pragma once
#include"UI/HUD.h"
#include"UI/TextBlock.h"
#include "UI/ProgressBar.h"
#include "UI/Image.h"
#include "UI/Button.h"

namespace we
{
	class Actor;

	class MainMenuHUD : public HUD
	{
	public:
		MainMenuHUD();

		virtual void Render(Renderer& GameRenderer) override;
		virtual void Tick(float DeltaTime) override;
		virtual bool HandleEvent(const optional<sf::Event> Event) override;
		Delegate<> OnStartButtonClicked;
		Delegate<> OnQuitButtonClicked;

	private:
		virtual void Initialize(Renderer& GameRenderer) override;
		Button StartButton;
		TextBlock StartButtonText;
		Button QuitButton;
		TextBlock QuitButtonText;
		void StartButtonClicked();
		void QuitButtonClicked();
		void InitializeButtons(const sf::Vector2u& ViewportSize);
	};
}
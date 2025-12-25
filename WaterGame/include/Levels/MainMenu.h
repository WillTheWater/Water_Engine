#pragma once
#include "Framework/World.h"

namespace we
{
	class MainMenuHUD;

	class MainMenu : public World
	{
	public:
		MainMenu(Application* OwningApp);

	private:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		weak<MainMenuHUD> MenuHUD;
		void StartGame();
		void QuitGame();
		void SetBackground();
	};
}
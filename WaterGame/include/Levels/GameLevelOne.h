#pragma once
#include "Framework/World.h"
#include "Framework/Core.h"
#include "Framework/TimerManager.h"

namespace we
{
	class PlayerSpaceship;
	class GameplayHUD;

	class LevelOne : public World
	{
	public:
		LevelOne(Application* OwningApp);
		void PlayerDied(Actor* Player);
		void GameOver();

	private:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		weak<PlayerSpaceship> APlayer;
		weak<GameplayHUD> GameplayHud;
		virtual void InitLevels() override;
	};
}
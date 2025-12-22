#pragma once
#include"UI/HUD.h"
#include"UI/TextBlock.h"
#include "UI/ProgressBar.h"
#include "UI/Image.h"

namespace we
{
	class Actor;

	class GameplayHUD : public HUD
	{
	public:
		GameplayHUD();

		virtual void Render(Renderer& GameRenderer) override;
		virtual void Tick(float DeltaTime) override;

	private:
		virtual void Initialize(Renderer& GameRenderer) override;
		void UpdatePlayerHealth(float Amount, float Current, float Max);
		void ResetHealthBar();
		void PlayerRespawn(Actor* Player);
		ProgressBar PlayerHealth;
		Image LifeIcon;
	};
}
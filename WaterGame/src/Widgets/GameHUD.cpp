#include "Widgets/GameHUD.h"
#include "Framework/Renderer.h"
#include "GameFramework/PlayerManager.h"
#include "Player/PlayerSpaceship.h"
#include "Player/Player.h"
#include "Framework/Actor.h"

namespace we
{
	GameplayHUD::GameplayHUD()
		: PlayerHealth{}
		, LifeIcon{ "SpaceShooterRedux/PNG/UI/playerLife1_orange.png" }
		, LifeText{""}
	{
	}

	void GameplayHUD::Render(Renderer& GameRenderer)
	{
		PlayerHealth.NativeRender(GameRenderer);
		LifeIcon.NativeRender(GameRenderer);
		LifeText.NativeRender(GameRenderer);
	}

	void GameplayHUD::Tick(float DeltaTime)
	{
	}

	void GameplayHUD::Initialize(Renderer& GameRenderer)
	{
		auto WindowSize = GameRenderer.GetViewportSize();
		PlayerHealth.SetWidgetPosition({ 140.f, WindowSize.y - PlayerHealth.GetBarSize().y});
		auto HealthPos = PlayerHealth.GetWidgetPosition();
		LifeIcon.SetWidgetPosition({ HealthPos.x + 110 , HealthPos.y - PlayerHealth.GetBarSize().y/2.f + 5});
		LifeIcon.ScaleImage({ 1.5f, 1.5f });
		LifeText.SetFontSize(32);
		LifeText.SetWidgetPosition({ LifeIcon.GetWidgetPosition().x + 60.f, LifeIcon.GetWidgetPosition().y - 10.f });
		ResetHealthBar();
		UpdatePlayerLifeCount();
	}

	void GameplayHUD::UpdatePlayerHealth(float Amount, float Current, float Max)
	{
		PlayerHealth.UpdateFillPercentage(Current, Max);
	}

	void GameplayHUD::ResetHealthBar()
	{
		Player* Player = PlayerManager::Get().GetPlayer();
		if (Player && !Player->GetPlayerSpaceship().expired())
		{
			weak<PlayerSpaceship> playerShip = Player->GetPlayerSpaceship();
			playerShip.lock()->OnActorDestroyed.Bind(GetObject(), &GameplayHUD::PlayerRespawn);
			HealthComponent& HealthComp = Player->GetPlayerSpaceship().lock()->GetHealthComponent();
			HealthComp.OnHealthChanged.Bind(GetObject(), &GameplayHUD::UpdatePlayerHealth);
			PlayerHealth.UpdateFillPercentage(HealthComp.GetCurrentHealth(), HealthComp.GetMaxHealth());
		}
	}

	void GameplayHUD::UpdatePlayerLifeCount()
	{
		Player* Player = PlayerManager::Get().GetPlayer();
		if (Player && !Player->GetPlayerSpaceship().expired())
		{
			int LifeCount = Player->GetLives();
			LifeText.SetText(std::to_string(LifeCount));
			Player->OnLivesChanged.Bind(GetObject(), &GameplayHUD::UpdateLives);
		}
	}

	void GameplayHUD::UpdateLives(int Amount)
	{
		LifeText.SetText(std::to_string(Amount));
	}

	void GameplayHUD::PlayerRespawn(Actor* Player)
	{
		ResetHealthBar();
	}
}
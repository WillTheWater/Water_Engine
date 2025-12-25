#include "Widgets/GameHUD.h"
#include "Framework/Renderer.h"
#include "GameFramework/PlayerManager.h"
#include "Player/PlayerSpaceship.h"
#include "Player/Player.h"
#include "Framework/Actor.h"
#include "Enemy/Boss.h"

namespace we
{
    GameplayHUD::GameplayHUD()
        : PlayerHealth{}
        , LifeIcon{ "SpaceShooterRedux/PNG/UI/playerLife1_orange.png" }
        , LifeText{ "" }
        , ScoreIcon{ "SpaceShooterRedux/PNG/Power-ups/powerupBlue.png" }
        , ScoreText{ "" }
        , WinLoseText{""}
        , RestartButton{ "SpaceShooterRedux/PNG/UI/buttonBlue.png" }
        , RestartButtonText{ "Restart" }
        , QuitButton{ "SpaceShooterRedux/PNG/UI/buttonBlue.png" }
        , QuitButtonText{ "Quit" }
    {
        WinLoseText.SetVisibility(false);
        RestartButton.SetVisibility(false);
        RestartButtonText.SetVisibility(false);
        QuitButton.SetVisibility(false);
        QuitButtonText.SetVisibility(false);
    }

    void GameplayHUD::Render(Renderer& GameRenderer)
    {
        PlayerHealth.NativeRender(GameRenderer);
        LifeIcon.NativeRender(GameRenderer);
        LifeText.NativeRender(GameRenderer);
        ScoreIcon.NativeRender(GameRenderer);
        ScoreText.NativeRender(GameRenderer);

        if (bBossBound)
        {
            BossHealthBar.NativeRender(GameRenderer);
        }
        if (true)
        {
            WinLoseText.NativeRender(GameRenderer);
            RestartButton.NativeRender(GameRenderer);
            RestartButtonText.NativeRender(GameRenderer);
            QuitButton.NativeRender(GameRenderer);
            QuitButtonText.NativeRender(GameRenderer);
        }
    }

    void GameplayHUD::Tick(float DeltaTime)
    {
        TrackBoss();
    }

    bool GameplayHUD::HandleEvent(const optional<sf::Event> Event)
    {
        return RestartButton.HandleEvent(Event) || QuitButton.HandleEvent(Event) || HUD::HandleEvent(Event);
    }

    void GameplayHUD::Initialize(Renderer& GameRenderer)
    {
        auto WindowSize = GameRenderer.GetViewportSize();
        RestartButton.OnButtonClicked.Bind(GetObject(), &GameplayHUD::RestartButtonClicked);
        QuitButton.OnButtonClicked.Bind(GetObject(), &GameplayHUD::QuitButtonClicked);

        PlayerHealth.SetWidgetPosition({ 140.f, WindowSize.y - PlayerHealth.GetBarSize().y });
        auto HealthPos = PlayerHealth.GetWidgetPosition();
        LifeIcon.SetWidgetPosition({ HealthPos.x + 110 , HealthPos.y - PlayerHealth.GetBarSize().y / 2.f + 5 });
        LifeIcon.ScaleImage({ 1.5f, 1.5f });
        LifeText.SetFontSize(32);
        LifeText.SetWidgetPosition({ LifeIcon.GetWidgetPosition().x + 60.f, LifeIcon.GetWidgetPosition().y - 10.f });
        ResetHealthBar();
        UpdatePlayerUIData();
        auto shift = ScoreIcon.GetGlobalBounds().size.x;
        ScoreIcon.SetWidgetPosition({ shift / 2.f, 30.f });
        BaseScoreIconWidth = ScoreIcon.GetGlobalBounds().size.x;
        ScoreText.SetFontSize(28);
        ScoreText.SetText("0");
        ScoreText.CenterOrigin();
        ScoreIcon.CenterOrigin();
        ScoreText.SetWidgetPosition(ScoreIcon.GetWidgetPosition());
        PositionScore();
        InitializeButtons(WindowSize);

        const float BossBarWidth = 600.f;
        const float BossBarHeight = 20.f;
        BossHealthBar.SetProgressBarDimensions({ BossBarWidth, BossBarHeight });

        float CenterX = WindowSize.x / 2.f;
        float BossBarY = 40.f;

        BossHealthBar.SetWidgetPosition({ CenterX, BossBarY });
        BossHealthBar.SetFillBarColor(sf::Color::Red);
    }

    void GameplayHUD::TrackBoss()
    {
        weak<Boss> BossRef = PlayerManager::Get().GetBoss();

        if (!BossRef.expired() && !bBossBound)
        {
            auto BossPtr = BossRef.lock();

            BossPtr->GetHealthComponent().OnHealthChanged.Bind(GetObject(), &GameplayHUD::UpdateBossHealth);

            BossPtr->OnActorDestroyed.Bind(GetObject(), &GameplayHUD::OnBossDestroyed);

            HealthComponent& HealthComp = BossPtr->GetHealthComponent();
            UpdateBossHealth(0, HealthComp.GetCurrentHealth(), HealthComp.GetMaxHealth());

            bBossBound = true;
        }
        else if (BossRef.expired() && bBossBound)
        {
            bBossBound = false;
        }
    }

    void GameplayHUD::UpdateBossHealth(float Amount, float Current, float Max)
    {
        BossHealthBar.UpdateFillPercentage(Current, Max);
        BossHealthBar.SetFillBarColor(sf::Color::Red);
    }

    void GameplayHUD::OnBossDestroyed(Actor* BossActor)
    {
        bBossBound = false;
    }

    void GameplayHUD::GameComplete(bool WinLose)
    {
        WinLoseText.SetVisibility(true);
        RestartButton.SetVisibility(true);
        RestartButtonText.SetVisibility(true);
        QuitButton.SetVisibility(true);
        QuitButtonText.SetVisibility(true);

        if (WinLose)
        {
            WinLoseText.SetText("You Win!");
            WinLoseText.CenterOrigin();
        }
        else
        {
            WinLoseText.SetText("You Lose!");
            WinLoseText.CenterOrigin();
        }
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

    void GameplayHUD::UpdatePlayerUIData()
    {
        Player* Player = PlayerManager::Get().GetPlayer();
        if (Player && !Player->GetPlayerSpaceship().expired())
        {
            int LifeCount = Player->GetLives();
            LifeText.SetText(std::to_string(LifeCount));
            Player->OnLivesChanged.Bind(GetObject(), &GameplayHUD::UpdateLives);
            int Score = Player->GetScore();
            ScoreText.SetText(std::to_string(Score));
            Player->OnScoreChanged.Bind(GetObject(), &GameplayHUD::ScoreUpdate);
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

    void GameplayHUD::PositionScore()
    {
        const float BaseIconScale = 1.5f;
        const float HorizontalPadding = 4.f;

        sf::FloatRect textBounds = ScoreText.GetLocalBounds();

        float desiredIconWidth =
            textBounds.size.x + HorizontalPadding * 2.f;

        float xScaleMultiplier = desiredIconWidth / BaseScoreIconWidth;

        ScoreIcon.ScaleImage({ 1.f, 1.f });
        ScoreIcon.ScaleImage({
            BaseIconScale * xScaleMultiplier,
            BaseIconScale
            });

        auto shift = ScoreIcon.GetGlobalBounds().size.x;
        sf::Vector2f pos{ shift / 2 + 30, 50.f };
        ScoreIcon.SetWidgetPosition(pos);
        ScoreText.SetWidgetPosition(pos);
    }

    void GameplayHUD::ScoreUpdate(int NewScore)
    {
        ScoreText.SetText(std::to_string(NewScore));
        ScoreText.CenterOrigin();
        PositionScore();
    }

    void GameplayHUD::InitializeButtons(const sf::Vector2u& ViewportSize)
    {
        RestartButton.CenterOrigin();
        RestartButtonText.SetFontSize(34);
        RestartButtonText.CenterOrigin();
        RestartButtonText.SetColor(sf::Color::Black);
        QuitButton.CenterOrigin();
        QuitButtonText.CenterOrigin();
        QuitButtonText.SetColor(sf::Color::Black);
        RestartButton.SetWidgetPosition({ ViewportSize.x / 2.f, ViewportSize.y / 2.f });
        RestartButtonText.SetWidgetPosition(RestartButton.GetWidgetPosition());
        QuitButton.SetWidgetPosition({ ViewportSize.x / 2.f, ViewportSize.y / 2.f + 100 });
        QuitButtonText.SetWidgetPosition(QuitButton.GetWidgetPosition());
        WinLoseText.SetFontSize(60);
        WinLoseText.SetWidgetPosition({ ViewportSize.x / 2.f, 300.f });
    }

    void GameplayHUD::RestartButtonClicked()
    {
        OnRestartButtonClicked.Broadcast();
    }

    void GameplayHUD::QuitButtonClicked()
    {
        OnQuitButtonClicked.Broadcast();
    }
}
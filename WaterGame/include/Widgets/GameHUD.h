#pragma once
#include"UI/HUD.h"
#include"UI/TextBlock.h"
#include "UI/ProgressBar.h"
#include "UI/Image.h"
#include "UI/Button.h"

namespace we
{
    class Actor;
    class Boss;

    class GameplayHUD : public HUD
    {
    public:
        GameplayHUD();

        virtual void Render(Renderer& GameRenderer) override;
        virtual void Tick(float DeltaTime) override;
        virtual bool HandleEvent(const optional<sf::Event> Event) override;

    private:
        virtual void Initialize(Renderer& GameRenderer) override;

        void UpdatePlayerHealth(float Amount, float Current, float Max);
        void ResetHealthBar();
        void UpdatePlayerUIData();
        void UpdateLives(int Amount);
        void PlayerRespawn(Actor* Player);
        void PositionScore();
        void ScoreUpdate(int NewScore);

        void TrackBoss();
        void UpdateBossHealth(float Amount, float Current, float Max);
        void OnBossDestroyed(Actor* BossActor);

        ProgressBar PlayerHealth;
        Image LifeIcon;
        TextBlock LifeText;
        Image ScoreIcon;
        TextBlock ScoreText;
        float BaseScoreIconWidth = 0.f;

        ProgressBar BossHealthBar;
        bool bBossBound = false;
    };
}
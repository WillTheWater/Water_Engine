#pragma once
#include "Enemy/EnemySpaceship.h"
#include "Weapons/BulletShooter.h"
#include "Weapons/TripleShot.h"
#include "Math.h"
#include "Framework/TimerManager.h"

namespace we
{
    class Boss : public EnemySpaceship
    {
    public:
        Boss(World* World);

        virtual void Tick(float DeltaTime) override;

    private:
        virtual void Shoot() override;
        void UpdateMovement(float DeltaTime);
        void UpdatePhases();

        void StartCharge();
        void OnChargeComplete();

        int CurrentPhase;
        bool bPhase2Activated;
        bool bPhase3Activated;

        bool bIsCharging;
        bool bIsLunging;
        bool bIsRecovering;
        float MoveDirectionX;

        float Speed;
        float SwitchDirectionX;
        float BaseY;
        float TimeTracker;

        sf::Vector2f LungeTarget;
        float LungeSpeed;
        float LungeTimer;

        TimerHandle ChargeTimerHandle;
        sf::Color OriginalColor;

        BulletShooter ShooterL;
        BulletShooter ShooterM;
        BulletShooter ShooterR;
        TripleShot ShooterTS;
        BulletShooter ShooterDiagL;
        BulletShooter ShooterDiagR;
    };
}
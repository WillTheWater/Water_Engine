#include "Enemy/Boss.h"
#include "Gameplay/HealthComponent.h"

namespace we
{
    Boss::Boss(World* World)
        : EnemySpaceship{ World, "SpaceShooterRedux/PNG/Enemies/enemyBlack1.png" }
        , Speed{ 200.f }
        , SwitchDirectionX{ 280.f }
        , BaseY{ 140.f }
        , TimeTracker{ 0.f }
        , CurrentPhase{ 1 }
        , bPhase2Activated{ false }
        , bPhase3Activated{ false }
        , bIsCharging{ false }
        , bIsLunging{ false }
        , bIsRecovering{ false }
        , MoveDirectionX{ 1.f }
        , LungeSpeed{ 1200.f }
        , LungeTimer{ 0.f }
        , OriginalColor{ sf::Color::White }
        , ShooterL{ this, 0.8f, {-100.f, 50}, sf::degrees(0) }
        , ShooterM{ this, 0.8f, {0.f, 60}, sf::degrees(0) }
        , ShooterR{ this, 0.8f, {100.f, 50}, sf::degrees(0) }
        , ShooterTS{ this, 1.2f, {0.f, 50}, sf::degrees(0) }
        , ShooterDiagL{ this, 0.4f, {-60.f, 40}, sf::degrees(-25) }
        , ShooterDiagR{ this, 0.4f, {60.f, 40}, sf::degrees(25) }
    {
        SetActorRotation(sf::degrees(90.f));
        SetSpriteRotationOffset(sf::degrees(-90));
        SetSpriteScale({ 2.3f, 2.3f });

        HealthComponent& HealthComp = GetHealthComponent();
        HealthComp.SetInitialHealth(1000.f, 1000.f);
    }

    void Boss::Tick(float DeltaTime)
    {
        EnemySpaceship::Tick(DeltaTime);

        UpdatePhases();
        UpdateMovement(DeltaTime);

        if (!bIsCharging && !bIsLunging)
        {
            Shoot();
            LungeTimer += DeltaTime;
        }
    }

    void Boss::UpdatePhases()
    {
        HealthComponent& HealthComp = GetHealthComponent();
        float Percent = HealthComp.GetHealthPercentage();

        if (Percent <= 66.f && !bPhase2Activated)
        {
            bPhase2Activated = true;
            CurrentPhase = 2;
            Speed = 300.f;
            LungeSpeed = 1600.f;

            ShooterM.AddWeaponLevel(4);
            ShooterTS.AddWeaponLevel(2);
        }

        if (Percent <= 33.f && !bPhase3Activated)
        {
            bPhase3Activated = true;
            CurrentPhase = 3;
            Speed = 400.f;
            LungeSpeed = 1800.f;

            ShooterL.AddWeaponLevel(3);
            ShooterM.AddWeaponLevel(5);
            ShooterR.AddWeaponLevel(3);
            ShooterTS.AddWeaponLevel(4);
        }
    }

    void Boss::StartCharge()
    {
        bIsCharging = true;
       GetSprite().setColor(sf::Color::Red);

        TimerManager::Get().SetTimer<Boss>(weak_from_this(), &Boss::OnChargeComplete, 1.0f);
    }

    void Boss::OnChargeComplete()
    {
        bIsCharging = false;
        GetSprite().setColor(OriginalColor);

        bIsLunging = true;

        sf::Vector2u WinSize = GetWindowSize();
        sf::Vector2f Loc = GetActorLocation();

        float TargetY = WinSize.y * 0.95f;
        float RandomX = (rand() % 300) - 150;
        LungeTarget = { Loc.x + RandomX, TargetY };
    }

    void Boss::UpdateMovement(float DeltaTime)
    {
        sf::Vector2f Loc = GetActorLocation();
        sf::Vector2u WinSize = GetWindowSize();

        if (CurrentPhase >= 2)
        {
            if (bIsCharging)
            {
                return;
            }

            if (bIsLunging)
            {
                sf::Vector2f Dir = LungeTarget - Loc;
                float Dist = std::sqrt(Dir.x * Dir.x + Dir.y * Dir.y);

                if (Dist < 15.f)
                {
                    bIsLunging = false;
                    bIsRecovering = true;
                    LungeTimer = 0.f;
                }
                else
                {
                    Dir /= Dist;
                    sf::Vector2f NewPos = Loc + (Dir * LungeSpeed * DeltaTime);
                    SetActorLocation(NewPos);
                }
                return;
            }

            if (bIsRecovering)
            {
                float ReturnSpeedY = 500.f;
                sf::Vector2f NewPos = Loc;

                if (Loc.y > BaseY)
                {
                    NewPos.y -= ReturnSpeedY * DeltaTime;
                }
                else
                {
                    NewPos.y = BaseY;
                    bIsRecovering = false;
                }

                NewPos.x += MoveDirectionX * Speed * DeltaTime;

                if (NewPos.x > WinSize.x - SwitchDirectionX) MoveDirectionX = -1.f;
                if (NewPos.x < SwitchDirectionX) MoveDirectionX = 1.f;

                SetActorLocation(NewPos);
                return;
            }

            float Cooldown = (CurrentPhase == 2) ? 6.f : 4.f;
            if (LungeTimer > Cooldown)
            {
                StartCharge();
                LungeTimer = 0.f;
            }
        }

        TimeTracker += DeltaTime;

        if (Loc.x > WinSize.x - SwitchDirectionX) MoveDirectionX = -1.f;
        else if (Loc.x < SwitchDirectionX) MoveDirectionX = 1.f;

        float Frequency = (CurrentPhase == 3) ? 6.f : 1.5f;
        float Amplitude = (CurrentPhase == 3) ? 40.f : 80.f;
        float YOffset = std::sin(TimeTracker * Frequency) * Amplitude;

        float NewX = Loc.x + (MoveDirectionX * Speed * DeltaTime);
        float NewY = BaseY + YOffset;

        SetActorLocation({ NewX, NewY });
    }

    void Boss::Shoot()
    {
        ShooterL.Shoot();
        ShooterM.Shoot();
        ShooterR.Shoot();

        if (CurrentPhase >= 2)
        {
            ShooterTS.Shoot();
        }

        if (CurrentPhase >= 3)
        {
            ShooterDiagL.Shoot();
            ShooterDiagR.Shoot();
        }
    }
}
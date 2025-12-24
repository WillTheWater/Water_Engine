#pragma once
#include "GameMode/Level.h"
#include "SFML/Graphics.hpp"
#include "Framework/TimerManager.h"

namespace we
{
    class Actor;
    class Boss;

    class BossLevel : public Level
    {
    public:
        BossLevel(World* World);

        virtual void BeginLevel() override;
        virtual void TickLevel(float DeltaTime) override;

    private:
        void BossDefeated(Actor* BossActor);
        void EntranceSequence(float DeltaTime);

        float EntranceTimer;
        weak<Boss> CurrentBoss;
    };
}
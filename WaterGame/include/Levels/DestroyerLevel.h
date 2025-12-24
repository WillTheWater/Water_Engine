#pragma once
#include "GameMode/Level.h"
#include "SFML/Graphics.hpp"
#include "Framework/TimerManager.h"

namespace we
{
    class DestroyerLevel : public Level
    {
    public:
        DestroyerLevel(World* World);

        virtual void BeginLevel() override;

    private:
        virtual void EndLevel() override;
        void SpawnDestroyer();

        float MinInterval;
        float MaxInterval;
        sf::Vector2f SpawnLocationL;
        sf::Vector2f SpawnLocationR;
        int DestroyerToSpawn;
        int CurrentDestroyerCount;

        TimerHandle SpawnTimerHandle;
    };
}
#pragma once
#include "Framework/Core.h"

namespace we
{
    enum class EActorID : uint8
    {
        Neutral = 0,

        // Player
        Player = 1,
        PlayerPawn = 2,
        PlayerProjectile = 3,

        // Enemy
        Enemy = 10,
        EnemyPawn = 11,
        EnemyProjectile = 12,

        // Neutral
        NPC = 20,

        // Environment
        WorldStatic = 30,
        WorldDynamic = 31,
        Obstacle = 32,

        // Items
        Pickup = 40,
        Collectible = 41,
        PowerUp = 42,

        // Effects
        Projectile = 50,
        Effect = 51,
        Trigger = 52,
    };

    enum class EActorSize
    {
        SpriteBounds,  // derived from current sprite bounds
        Manual,        // explicitly set individually
        PhysicsShape   // Not Implimented
    };

}
// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOneBarriers.h"
#include "Framework/World/Barrier.h"
#include "Framework/World/World.h"

namespace we
{
    void CreateLevelOneBarriers(World& InWorld)
    {
        // Bottom boundary
        auto BottomBarrier = InWorld.SpawnActor<Barrier>().lock();
        BottomBarrier->SetPoints({
            {0.0f, 2855.0f},
            {5760.0f, 2855.0f}
        });
        BottomBarrier->SetThickness(8.0f);

        // Forest/world boundary
        auto ForestBarrier = InWorld.SpawnActor<Barrier>().lock();
        ForestBarrier->SetPoints({
            {0, 3400},
            {0.0f, 980},
            {950, 400},
            {1600, 220},
            {2200, 316},
            {2680, 290},
            {2950, 0},
            {3295, 0},
            {3510, 370},
            {4230, 460},
            {4970, 330},
            {5415, 480},
            {5760, 1080},
            {5760, 3400}
        });
        ForestBarrier->SetThickness(8.0f);
        ForestBarrier->SetClosed(true);

        // Hut 1 collision
        auto Hut1Barrier = InWorld.SpawnActor<Barrier>().lock();
        Hut1Barrier->SetPoints({
            {215, 1350},
            {865, 940},
            {1265, 1295},
            {915, 1600}
        });
        Hut1Barrier->SetThickness(8.0f);
        Hut1Barrier->SetClosed(true);

        // Hut 2 collision
        auto Hut2Barrier = InWorld.SpawnActor<Barrier>().lock();
        Hut2Barrier->SetPoints({
            {1314, 850},
            {958, 566},
            {1669, 400},
            {1945, 745}
        });
        Hut2Barrier->SetThickness(8.0f);
        Hut2Barrier->SetClosed(true);

        // Hut 3 collision
        auto Hut3Barrier = InWorld.SpawnActor<Barrier>().lock();
        Hut3Barrier->SetPoints({
            {3425, 735},
            {3700, 800},
            {3822, 989},
            {3709, 989},
            {3709, 1100},
            {4000, 1100},
            {4000, 980},
            {4380, 980},
            {4400, 1050},
            {4500, 1050},
            {4340, 500}
        });
        Hut3Barrier->SetThickness(8.0f);
        Hut3Barrier->SetClosed(true);
    }
}

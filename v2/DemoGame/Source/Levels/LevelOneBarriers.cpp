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
        auto ForestBarrier = InWorld.SpawnActor<Barrier>().lock();
        ForestBarrier->SetPoints({
            {0, 2855.0f},
            {0.0f, 1068},
            {70, 1068},
            {192, 1002},
            {415, 1105},
            {568, 1091},
            {772, 1202},
            {1007, 1119},
            {1260, 948},
            {1260, 854},
            {614, 500},
            {780, 400},
            {1110, 217},
            {1713, 155},
            {1834, 541},
            {1528, 700},
            {1664, 853},
            {1941, 933},
            {2143, 940},
            {2570, 734},
            {2015, 415},
            {2234, 215},
            {2794, 210},
            {2898, 0},
            {3360, 0},
            {3720, 470},
            {3399, 640},
            {3720, 870},
            {3985, 882},
            {4412, 660},
            {3905, 410},
            {4080, 120},
            {4389, 242},
            {4404, 481},
            {4640, 481},
            {4702, 372},
            {5119, 320},
            {5422, 320},
            {5570, 445},
            {5535, 600},
            {5760, 700},
            {5760, 2855.0f},
        });
        ForestBarrier->SetThickness(8.0f);
        ForestBarrier->SetClosed(true);
    }
}
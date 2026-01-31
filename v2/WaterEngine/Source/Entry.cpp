// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <SFML/GpuPreference.hpp>
#include <SFML/System/Sleep.hpp>

#include "Entry.h"
#include "Framework/WaterEngine.h"

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main()
{
	auto WE = we::GetEngine();
    while (WE->IsRunning())
    {
        WE->ProcessEvents();
        if (!WE->HasFocus())
        {
            sf::sleep(sf::microseconds(10));
            continue;
        }
        WE->GlobalTick();
        WE->Render();
        WE->PostUpdate();
    }
}
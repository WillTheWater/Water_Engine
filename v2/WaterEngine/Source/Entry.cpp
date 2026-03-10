// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <SFML/System/Sleep.hpp>
#include <SFML/GpuPreference.hpp>

#include "Entry.h"
#include "Framework/WaterEngine.h"

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main()
{
	auto Engine = we::GetEngine();
	Engine->StartPlay();

	while (Engine->IsRunning())
	{
		Engine->ProcessEvents();

		if (!Engine->HasFocus())
		{
			sf::sleep(sf::microseconds(10));
			continue;
		}

		Engine->Update();
		Engine->Render();
		Engine->ProcessQuit();
	}
	return 0;
}
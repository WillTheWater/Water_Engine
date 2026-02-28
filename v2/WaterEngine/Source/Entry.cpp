// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <SFML/GpuPreference.hpp>

#include "Entry.h"
#include "Framework/WaterEngine.h"

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main()
{
	auto Engine = we::GetEngine();
	Engine->Initialize();

	while (Engine->IsRunning())
	{
		Engine->ProcessEvents();
		Engine->Tick();
		Engine->Render();
	}

	return 0;
}

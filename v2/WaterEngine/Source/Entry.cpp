// =============================================================================
// Water Engine v2.1.2
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <SFML/System/Sleep.hpp>
#include <SFML/GpuPreference.hpp>

#include "Entry.h"
#include "Framework/WaterEngine.h"

#if defined(NDEBUG) && defined(WIN32)
#include "Utility/SplashScreen.h"
#endif

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main()
{
#if defined(NDEBUG) && defined(WIN32)
	we::ShowSplash();
#endif

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
	
	Engine->Shutdown();
	return 0;
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"
#include "Tests/WidgetTests.h"
#include "UI/MainMenuUI.h"

namespace we
{
    MainMenu::MainMenu(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    void MainMenu::BeginPlay()
    {		
		// Main Menu UI
        MainMenuUI MenuUI;
		MenuUI.Initialize();
		MenuUI.Show();
    }

    void MainMenu::Tick(float DeltaTime)
    {
      
    }
}

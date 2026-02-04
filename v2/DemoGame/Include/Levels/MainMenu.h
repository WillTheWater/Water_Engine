// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Framework/World/World.h"
#include "Utility/Delegate.h"

namespace we
{
    class MainMenu : public World
    {
    public:
        explicit MainMenu(EngineSubsystem& Subsystem);
        ~MainMenu();
        virtual void Render() override;

        Delegate<> OnPlayPressed;
        Delegate<> OnQuitPressed;
    private:
        shared<texture> BgTexture;
        optional<sprite> Background;

    private:
        void CreateUI();
        void HandlePlay();
        void HandleQuit();
    };
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "Utility/Log.h"

namespace we
{
    WaterEngine::WaterEngine()
    {
        Initialize();
    }

    void WaterEngine::Initialize()
    {
        Subsystem.Window = make_unique<WindowSubsystem>();
        Subsystem.Clock  = make_unique<ClockSubsystem>();
    }

    bool WaterEngine::IsRunning() const
    {
        return Subsystem.Window->isOpen();
    }

    bool WaterEngine::HasFocus() const
    {
        return Subsystem.Window->hasFocus();
    }

    void WaterEngine::ProcessEvents()
    {
        while (const auto Event = Subsystem.Window->pollEvent())
        {
            Subsystem.Window->HandleEvent(*Event);
        }
    }

    void WaterEngine::Update()
    {
        Subsystem.Clock->Tick();
    }

    void WaterEngine::Render()
    {
    }
}

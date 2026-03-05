// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "Utility/Delegate.h"
#include "Utility/Log.h"
#include "Utility/Math.h"

namespace we
{
    WaterEngine::WaterEngine()
    {
        Initialize();
    }

    void WaterEngine::Initialize()
    {
        Subsystem.Resource = make_unique<ResourceSubsystem>();
        Subsystem.Window   = make_unique<WindowSubsystem>();
        Subsystem.Clock    = make_unique<ClockSubsystem>();
        Subsystem.Render   = make_unique<RenderSubsystem>();
        Subsystem.Camera   = make_unique<CameraSubsystem>();

        BindDelegates();
    }

    void WaterEngine::BindDelegates()
    {
        Subsystem.Window->OnResize.Bind(this, &WaterEngine::SetCameraView);
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
        Subsystem.Render->BeginFrame();

        // TODO: Draw Call

        Subsystem.Render->EndFrame();

        Subsystem.Window->setView(Subsystem.Camera->GetView());
        
        Subsystem.Window->clear(color::Black);
        Subsystem.Window->draw(Subsystem.Render->GetCompositeSprite());
        Subsystem.Window->display();
    }

    void WaterEngine::SetCameraView(vec2u WindowSize)
    {
        Subsystem.Camera->Update(WindowSize);
    }
}
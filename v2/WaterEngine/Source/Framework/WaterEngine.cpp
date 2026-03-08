// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "Utility/Delegate.h"
#include "Utility/Log.h"
#include "Utility/Math.h"
#include "Subsystem/GuiSubsystem.h"

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
        Subsystem.Timer    = make_unique<TimerSubsystem>();
        Subsystem.World    = make_unique<WorldSubsystem>();
        Subsystem.Render   = make_unique<RenderSubsystem>();
        Subsystem.Camera   = make_unique<CameraSubsystem>();
        Subsystem.Cursor   = make_unique<CursorSubsystem>();
        Subsystem.Input    = make_unique<InputSubsystem>();
        Subsystem.GUI      = make_unique<GUISubsystem>();

        BindDelegates();
    }

    void WaterEngine::BindDelegates()
    {
        Subsystem.Window->OnResize.Bind(Subsystem.Camera.get(), &CameraSubsystem::SetCameraView);
        Subsystem.Window->OnMouseMove.Bind(Subsystem.Cursor.get(), &CursorSubsystem::SetPosition);
        Subsystem.Camera->OnViewUpdate.Bind(Subsystem.GUI.get(), &GUISubsystem::SetCameraView);
        GetTimer().TriggerGarbageCollection.Bind(Subsystem.Resource.get(), &ResourceSubsystem::GarbageCollect);
        Subsystem.GUI->Initialize(Subsystem.Render->GetScreenUITarget(), Subsystem.Render->GetWorldUITarget());
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

            if (Subsystem.GUI->HandleEvent(*Event)) { continue; }

            Subsystem.Input->HandleEvent(*Event);
           
        }
    }

    void WaterEngine::Update()
    {
        Subsystem.Clock->Tick();
        float DeltaTime = Subsystem.Clock->GetDeltaTime();
        GetTimer().Update(DeltaTime);
        Subsystem.World->Tick(DeltaTime);
    }

    void WaterEngine::Render()
    {
        Subsystem.Render->BeginFrame();

        // World layer
        for (const auto* Sprite : Subsystem.World->GetOrderedDrawables())
        {
            Subsystem.Render->Draw(*Sprite, ERenderLayer::World);
        }

        // WorldUI layer
        Subsystem.GUI->GetWorldUI().draw();

        // ScreenUI layer
        Subsystem.GUI->GetScreenUI().draw();

        // Cursor layer
        if (const auto* CursorDrawable = Subsystem.Cursor->GetDrawable())
        {
            Subsystem.Render->Draw(*CursorDrawable, ERenderLayer::Cursor);
        }

        Subsystem.Render->EndFrame();

        Subsystem.Window->setView(Subsystem.Camera->GetView());
        
        Subsystem.Window->clear(color::Black);
        Subsystem.Window->draw(Subsystem.Render->GetCompositeSprite());
        Subsystem.Window->display();
    }
}
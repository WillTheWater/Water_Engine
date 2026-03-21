// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"

namespace we
{
    WaterEngine::WaterEngine()
    {
        Initialize();
    }
    
    WaterEngine::~WaterEngine()
    {
        if (Subsystem.GameInstance)
        {
            Subsystem.GameInstance->Shutdown();
            Subsystem.GameInstance.reset();
        }
    }

    void WaterEngine::Initialize()
    {
        Subsystem.Resource = make_unique<ResourceSubsystem>();
        Subsystem.Audio    = make_unique<AudioSubsystem>();
        Subsystem.Window   = make_unique<WindowSubsystem>();
        Subsystem.Clock    = make_unique<ClockSubsystem>();
        Subsystem.Timer    = make_unique<TimerSubsystem>();
        Subsystem.World    = make_unique<WorldSubsystem>();
        Subsystem.Render   = make_unique<RenderSubsystem>();
        Subsystem.Camera   = make_shared<CameraSubsystem>();
        Subsystem.Cursor   = make_unique<CursorSubsystem>();
        Subsystem.Input    = make_unique<InputSubsystem>();
        Subsystem.GUI      = make_unique<GUISubsystem>();
        Subsystem.Save     = make_unique<SaveSubsystem>();
        Subsystem.Physics  = make_shared<PhysicsSubsystem>();

        BindDelegates();
    }

    void WaterEngine::BindDelegates()
    {
        Subsystem.Window->OnResize.Bind(Subsystem.Camera.get(), &CameraSubsystem::SetCameraView);
        Subsystem.Window->OnMouseMove.Bind(Subsystem.Cursor.get(), &CursorSubsystem::SetPosition);
        Subsystem.Window->OnResize.Bind(Subsystem.GUI.get(), &GUISubsystem::SetWindowSize);
        Subsystem.Camera->OnViewUpdate.Bind(Subsystem.GUI.get(), &GUISubsystem::SetCameraView);
        GetTimer().TriggerGarbageCollection.Bind(Subsystem.Resource.get(), &ResourceSubsystem::GarbageCollect);
        Subsystem.GUI->Initialize(Subsystem.Render->GetScreenUITarget(), Subsystem.Render->GetWorldUITarget());
        Subsystem.GUI->SetWindowSize(Subsystem.Window->getSize());
        Subsystem.World->SetPhysicsRef(Subsystem.Physics);
        Subsystem.World->SetCameraRef(Subsystem.Camera);
    }

    void WaterEngine::StartPlay()
    {
        Subsystem.GameInstance = CreateGameInstance();
        if (Subsystem.GameInstance) { Subsystem.GameInstance->Init(Subsystem); }
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
        
        if (Subsystem.World->IsPaused() && !Subsystem.Clock->IsPaused())
        {
            Subsystem.Clock->Pause();
        }
        else if (!Subsystem.World->IsPaused() && Subsystem.Clock->IsPaused())
        {
            Subsystem.Clock->Resume();
        }
        
        float DeltaTime = Subsystem.Clock->GetDeltaTime();
        GetTimer().Update(DeltaTime);
        Subsystem.World->Tick(DeltaTime);
        Subsystem.Physics->Tick(DeltaTime);
        Subsystem.Audio->Update(DeltaTime);
    }

    void WaterEngine::ProcessQuit()
    {
        if (Subsystem.World->ShouldQuit())
        {
            Subsystem.Window->close();
        }
    }

    void WaterEngine::Render()
    {
        // Get camera view data from CameraSubsystem
        vec2f CamPos = Subsystem.Camera->GetViewPosition();
        float CamZoom = Subsystem.Camera->GetViewZoom();
        float CamRot = Subsystem.Camera->GetViewRotation();
        
        Subsystem.Render->SetWorldView(CamPos, CamZoom, CamRot);
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
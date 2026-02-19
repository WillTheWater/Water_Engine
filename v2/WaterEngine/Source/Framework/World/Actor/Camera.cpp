// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Camera.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
    Camera::Camera(World* OwningWorld)
        : Actor(OwningWorld, "")  // No texture for camera
    {
    }

    Camera::~Camera() = default;

    void Camera::BeginPlay()
    {
        Actor::BeginPlay();
    }

    void Camera::Destroy()
    {
        // If this was the active camera, clear it
        if (GetWorld())
        {
            auto* CamSys = GetWorld()->GetSubsystem().Camera.get();
            if (CamSys && CamSys->GetActiveCamera() == this)
            {
                CamSys->SetActiveCamera(nullptr);
            }
        }
        
        Actor::Destroy();
    }

    void Camera::SetViewHeight(float Height)
    {
        ViewHeight = Height;
    }

    void Camera::SetZoom(float NewZoom)
    {
        Zoom = NewZoom;
    }

    CameraView Camera::CalculateView() const
    {
        CameraView View;
        View.Position = GetPosition();
        View.Rotation = GetRotation().asRadians();
        View.OrthographicSize = ViewHeight;
        View.Zoom = Zoom;
        
        return View;
    }
}

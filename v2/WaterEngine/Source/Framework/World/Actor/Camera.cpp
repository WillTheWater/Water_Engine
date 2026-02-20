// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Camera.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"

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
        LOG("Camera Begin Play")
    }

    void Camera::Tick(float DeltaTime)
    {
        Actor::Tick(DeltaTime);
        
        // Base class Tick() guards against ticking before BeginPlay
        // Only update position if we've begun play
        if (!HasBegunPlay()) return;
        
        UpdatePosition(DeltaTime);
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

    void Camera::AttachTo(Actor* Target, vec2f LocalOffset)
    {
        TargetActor = Target;
        TargetOffset = LocalOffset;
        Velocity = {0, 0};
    }

    void Camera::Detach()
    {
        TargetActor = nullptr;
        Velocity = {0, 0};
    }

    void Camera::UpdatePosition(float DeltaTime)
    {
        if (!TargetActor) return;

        vec2f TargetPos = TargetActor->GetPosition() + TargetOffset;
        vec2f CurrentPos = GetPosition();

        if (SmoothTime <= 0.0f)
        {
            // Instant follow (no lag)
            SetPosition(TargetPos);
        }
        else
        {
            // Lerp towards target - higher SmoothTime = more lag
            float t = Clamp(DeltaTime / SmoothTime, 0.0f, 1.0f);
            vec2f NewPos = LerpVector(CurrentPos, TargetPos, t);
            SetPosition(NewPos);
        }
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

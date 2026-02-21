// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interface/Component/CameraComponent.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"

namespace we
{
    CameraComponent::CameraComponent(Actor* InOwner)
        : Owner(InOwner)
        , CameraPosition(InOwner ? InOwner->GetPosition() : vec2f{0, 0})
    {
    }

    CameraComponent::~CameraComponent()
    {
        // If this was the active camera, clear it
        if (Owner && Owner->GetWorld())
        {
            auto* CamSys = Owner->GetWorld()->GetSubsystem().Camera.get();
            if (CamSys && CamSys->GetActiveCamera() == this)
            {
                CamSys->SetActiveCamera(nullptr);
            }
        }
    }

    void CameraComponent::Update(float DeltaTime)
    {
        UpdatePosition(DeltaTime);
    }

    void CameraComponent::SetActive()
    {
        if (Owner && Owner->GetWorld())
        {
            Owner->GetWorld()->GetSubsystem().Camera->SetActiveCamera(this);
        }
    }

    bool CameraComponent::IsActive() const
    {
        if (!Owner || !Owner->GetWorld()) return false;
        return Owner->GetWorld()->GetSubsystem().Camera->GetActiveCamera() == this;
    }

    void CameraComponent::AttachTo(Actor* Target, vec2f LocalOffset)
    {
        TargetActor = Target;
        TargetOffset = LocalOffset;
        
        // Initialize camera position at target position + offset
        if (Target)
        {
            CameraPosition = Target->GetPosition() + LocalOffset;
        }
    }

    void CameraComponent::Detach()
    {
        TargetActor = nullptr;
    }

    void CameraComponent::UpdatePosition(float DeltaTime)
    {
        if (!TargetActor) return;

        // Target is the actor position + offset
        vec2f TargetPos = TargetActor->GetPosition() + TargetOffset;
        vec2f OldCamPos = CameraPosition;

        if (SmoothTime <= 0.0f)
        {
            // Instant follow (no lag)
            CameraPosition = TargetPos;
        }
        else
        {
            // Lerp towards target - higher SmoothTime = more lag
            float t = Clamp(DeltaTime / SmoothTime, 0.0f, 1.0f);
            CameraPosition = LerpVector(CameraPosition, TargetPos, t);
        }
        

    }

    CameraView CameraComponent::CalculateView() const
    {
        CameraView View;
        View.Position = CameraPosition;
        View.Rotation = Owner ? Owner->GetRotation().asRadians() : 0.0f;
        View.OrthographicSize = ViewHeight;
        View.Zoom = Zoom;
        

        return View;
    }
}

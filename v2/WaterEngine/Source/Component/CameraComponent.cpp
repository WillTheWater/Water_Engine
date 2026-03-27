// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/CameraComponent.h"
#include "Framework/World/Actor.h"
#include "Framework/World/World.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/CameraSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"
#include <cmath>

namespace we
{
    CameraComponent::CameraComponent(Actor* InOwner)
        : Owner(InOwner)
        , SmoothedPosition(InOwner ? InOwner->GetPosition() : vec2f{0.0f, 0.0f})
        , AttachedTarget(nullptr)  // Don't follow anyone by default
    {
    }

    CameraComponent::~CameraComponent() = default;

    void CameraComponent::BeginPlay()
    {
        if (Owner)
        {
            Subsystem = &Owner->GetWorld().GetCamera();
        }
    }

    void CameraComponent::Tick(float DeltaTime)
    {
        UpdatePosition(DeltaTime);
    }

    void CameraComponent::EndPlay()
    {
        if (IsActive() && Subsystem)
        {
            Subsystem->ClearActiveCamera();
        }
    }

    Actor* CameraComponent::GetOwner() const
    {
        return Owner;
    }

    void CameraComponent::SetActive()
    {
        if (!Subsystem && Owner)
        {
            Subsystem = &Owner->GetWorld().GetCamera();
        }

        if (Subsystem)
        {
            Subsystem->SetActiveCamera(this);
        }
    }

    bool CameraComponent::IsActive() const
    {
        if (!Subsystem) return false;
        return Subsystem->GetActiveCamera() == this;
    }

    void CameraComponent::SetOffset(vec2f InOffset)
    {
        Offset = InOffset;
    }

    void CameraComponent::SetZoom(float InZoom)
    {
        Zoom = InZoom;
    }

    void CameraComponent::SetRotation(float InRadians)
    {
        Rotation = InRadians;
    }

    void CameraComponent::AttachTo(Actor* TargetActor)
    {
        AttachedTarget = TargetActor;
    }

    void CameraComponent::SetSmoothFollow(bool bEnable, float InSmoothTime)
    {
        bSmoothFollow = bEnable;
        SmoothTime = InSmoothTime;
    }

    void CameraComponent::Shake(float Amplitude, float Duration, float Frequency)
    {
        ActiveShake = ShakeState{Amplitude, Duration, Frequency, 0.0f};
    }

    void CameraComponent::StopShake()
    {
        ActiveShake.reset();
    }

    void CameraComponent::SetBounds(rectf WorldBounds)
    {
        Bounds = WorldBounds;
    }

    void CameraComponent::ClearBounds()
    {
        Bounds.reset();
    }

    vec2f CameraComponent::GetFinalPosition() const
    {
        return SmoothedPosition;
    }

    void CameraComponent::UpdatePosition(float DeltaTime)
    {
        if (!AttachedTarget) return;
        vec2f TargetPos = AttachedTarget->GetPosition() + Offset;

        if (bSmoothFollow)
        {
            float t = Clamp(DeltaTime / SmoothTime, 0.0f, 1.0f);
            SmoothedPosition = LerpVector(SmoothedPosition, TargetPos, t);
        }
        else
        {
            SmoothedPosition = TargetPos;
        }

        if (ActiveShake)
        {
            ActiveShake->Elapsed += DeltaTime;
            if (ActiveShake->Elapsed >= ActiveShake->Duration)
            {
                ActiveShake.reset();
            }
            else
            {
                SmoothedPosition = SmoothedPosition + ApplyShake();
            }
        }

        if (Bounds)
        {
            ClampToBounds(SmoothedPosition);
        }
    }

    vec2f CameraComponent::ApplyShake() const
    {
        if (!ActiveShake) return {0.0f, 0.0f};

        float progress = ActiveShake->Elapsed / ActiveShake->Duration;
        float currentAmplitude = ActiveShake->Amplitude * (1.0f - progress);
        float angle = ActiveShake->Elapsed * ActiveShake->Frequency * 2.0f * 3.14159f;
        float x = std::cos(angle) * currentAmplitude;
        float y = std::sin(angle * 1.3f) * currentAmplitude;
        
        return {x, y};
    }

    void CameraComponent::ClampToBounds(vec2f& Position) const
    {
        if (!Bounds) return;
        
        Position.x = Clamp(Position.x, Bounds->position.x, 
            Bounds->position.x + Bounds->size.x);
        Position.y = Clamp(Position.y, Bounds->position.y,
            Bounds->position.y + Bounds->size.y);
    }
}
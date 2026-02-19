// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Subsystem/CameraSubsystem.h"

namespace we
{
    class Actor;

    // =========================================================================
    // CameraComponent - Camera behavior as a component (not an Actor)
    // Owned by an Actor, updated by that Actor's Tick()
    // =========================================================================
    class CameraComponent : public ICamera
    {
    public:
        explicit CameraComponent(Actor* Owner);
        ~CameraComponent();

        // Called by owning actor's Tick()
        void Update(float DeltaTime);

        // Calculate view from this camera
        CameraView CalculateView() const;

        // Set as active camera
        void SetActive();
        bool IsActive() const;

        // Attachment - follows target actor (can be owner or different actor)
        void AttachTo(Actor* Target, vec2f LocalOffset = {0, 0});
        void Detach();
        bool IsAttached() const { return TargetActor != nullptr; }

        // Settings
        void SetViewHeight(float Height) { ViewHeight = Height; }
        void SetZoom(float NewZoom) { Zoom = NewZoom; }
        float GetZoom() const { return Zoom; }
        void SetSmoothTime(float Time) { SmoothTime = Time; }
        float GetSmoothTime() const { return SmoothTime; }

    private:
        void UpdatePosition(float DeltaTime);

    private:
        Actor* Owner;

        // Camera position (separate from owner - allows offset)
        vec2f CameraPosition;

        // View settings
        float ViewHeight = 540.0f;
        float Zoom = 1.0f;
        
        // Attachment
        Actor* TargetActor = nullptr;
        vec2f TargetOffset;
        float SmoothTime = 0.0f;
    };
}

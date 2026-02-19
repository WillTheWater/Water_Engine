// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor/Actor.h"
#include "Subsystem/CameraSubsystem.h"

namespace we
{
    // =========================================================================
    // Camera - An actor that provides a view for rendering
    // =========================================================================
    class Camera : public Actor, public ICamera
    {
    public:
        explicit Camera(World* OwningWorld);
        virtual ~Camera();

        // Lifecycle
        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;
        virtual void Destroy() override;

        // Calculate the current view from this camera
        CameraView CalculateView() const;

        // Settings
        void SetViewHeight(float Height);  // Half-height in world units (default 540)
        void SetZoom(float NewZoom);       // 1.0 = normal, 2.0 = 2x zoomed in
        float GetZoom() const { return Zoom; }

        // Attachment - camera follows target actor
        void AttachTo(Actor* Target, vec2f LocalOffset = {0, 0});
        void Detach();
        bool IsAttached() const { return TargetActor != nullptr; }

        // Follow settings
        void SetSmoothTime(float Time) { SmoothTime = Time; }  // 0 = instant, >0 = smooth
        float GetSmoothTime() const { return SmoothTime; }

    private:
        void UpdatePosition(float DeltaTime);

    private:
        // View settings
        float ViewHeight = 540.0f;
        float Zoom = 1.0f;
        
        // Attachment
        Actor* TargetActor = nullptr;
        vec2f TargetOffset;
        float SmoothTime = 0.0f;  // 0 = instant follow
        vec2f Velocity;           // For smooth damp
    };
}

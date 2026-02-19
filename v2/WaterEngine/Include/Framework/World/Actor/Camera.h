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
    class Camera : public Actor
    {
    public:
        explicit Camera(World* OwningWorld);
        virtual ~Camera();

        // Lifecycle
        virtual void BeginPlay() override;
        virtual void Destroy() override;

        // Calculate the current view from this camera
        CameraView CalculateView() const;

        // Settings
        void SetViewHeight(float Height);  // Half-height in world units (default 540)
        void SetZoom(float NewZoom);       // 1.0 = normal, 2.0 = 2x zoomed in
        float GetZoom() const { return Zoom; }

    private:
        // Half-height of view in world units
        // At Zoom=1.0 and ViewHeight=540, view spans from y=-540 to y=+540
        float ViewHeight = 540.0f;
        float Zoom = 1.0f;
    };
}

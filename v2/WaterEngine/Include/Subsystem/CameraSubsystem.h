// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    // =========================================================================
    // CameraView - The output of a camera, fed to the renderer
    // =========================================================================
    struct CameraView
    {
        vec2f Position = {0.0f, 0.0f};        // Center of view in world space
        float Rotation = 0.0f;                 // Rotation in radians
        float OrthographicSize = 540.0f;       // Half-height in world units (1080p/2 default)
        float Zoom = 1.0f;                     // 1.0 = normal, 2.0 = 2x zoomed in

        // Calculate final view size with zoom applied
        vec2f GetViewSize(float AspectRatio) const
        {
            float Height = (OrthographicSize * 2.0f) / Zoom;
            float Width = Height * AspectRatio;
            return {Width, Height};
        }
    };

    // =========================================================================
    // CameraSubsystem - Manages cameras and provides view to renderer
    // =========================================================================
    class Camera;

    class CameraSubsystem
    {
    public:
        CameraSubsystem() = default;
        ~CameraSubsystem() = default;

        // Set which camera is active (nullptr = none)
        void SetActiveCamera(Camera* Cam);
        Camera* GetActiveCamera() const;

        // Get the current view (for renderer)
        // Returns true if a valid view exists, false otherwise
        bool GetCurrentView(CameraView& OutView) const;

        // Returns true if we have an active camera
        bool HasActiveCamera() const;

    private:
        // Currently active camera
        Camera* ActiveCamera = nullptr;
    };
}

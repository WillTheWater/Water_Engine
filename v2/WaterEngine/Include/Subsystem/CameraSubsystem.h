// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Utility/Delegate.h"

namespace we
{
    class CameraComponent;

    class CameraSubsystem
    {
    public:
        CameraSubsystem();
        ~CameraSubsystem() = default;

        // RenderSubsystem integration
        void SetCameraView(vec2u WindowSize);
        const view& GetView() const;
        Delegate<const view&> OnViewUpdate;

        // Active camera management
        void SetActiveCamera(CameraComponent* Camera);
        void ClearActiveCamera();  // Use default view
        CameraComponent* GetActiveCamera() const { return ActiveCamera; }
        bool HasActiveCamera() const { return ActiveCamera != nullptr; }

        // View data
        vec2f GetViewPosition() const;
        float GetViewZoom() const;
        float GetViewRotation() const;

        // Default view settings
        void SetDefaultView(vec2f Center, float Zoom = 1.0f, float Rotation = 0.0f);

    private:
        CameraComponent* ActiveCamera = nullptr;
        view CameraView;
        vec2f RenderResolution;

        // Default view when no camera is active
        vec2f DefaultPosition = {960.0f, 540.0f};
        float DefaultZoom = 1.0f;
        float DefaultRotation = 0.0f;
    };
}
// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CameraSubsystem.h"
#include "Framework/World/Actor/Camera.h"

namespace we
{
    void CameraSubsystem::SetActiveCamera(Camera* Cam)
    {
        ActiveCamera = Cam;
    }

    Camera* CameraSubsystem::GetActiveCamera() const
    {
        return ActiveCamera;
    }

    bool CameraSubsystem::GetCurrentView(CameraView& OutView) const
    {
        if (!ActiveCamera) return false;
        
        OutView = ActiveCamera->CalculateView();
        return true;
    }

    bool CameraSubsystem::HasActiveCamera() const
    {
        return ActiveCamera != nullptr;
    }
}
